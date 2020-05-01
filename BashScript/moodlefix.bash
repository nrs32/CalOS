 #!/bin/bash
LANG=C

isVerbose=false
inputFile=""

# Determine if user added verbose argument.
if [[ "$1" = "-v" || "$1" = "--verbose" ]] ; then
    isVerbose=true
    inputFile="${@:2}" # All but first arg
else 
    inputFile="$@"
fi

removeSpaces() {
    echo "$1" | sed -e "s/ //g"
}

removeDirTrailingCruft() {
    echo "$1" | sed -e "s/-[0-9]*.zip//g"
}

removeSubdirTrailingCruft() {
    echo "$1" | sed -e "s/_[0-9]*_.*//g"
}

# Create directory for results
noSpaces=$(removeSpaces "$inputFile")
resultDir=$(removeDirTrailingCruft "$noSpaces")

[[ $isVerbose = true ]] && echo "Creating directory for results called $resultDir" && echo
mkdir "$resultDir"

# Unzip original directory into result directoy
[[ $isVerbose = true ]] && echo "Unzipping file into $resultDir" && echo
unzip -q "$inputFile" -d "$resultDir" # -q suppresses unzip output

# Rename all directories and files inside resultDir
# unzip and combine directories as needed
subdirs="$resultDir/*"
for dir in $subdirs
do
    # We must ignore spaces in dir
    # so IFS should only care about new lines
    defaultIFS="$IFS"
    IFS=$'\n' 

    noSpacesDir=$(removeSpaces "$dir")
    newDirName=$(removeSubdirTrailingCruft "$noSpacesDir")
    
    # If newDirName already exists
    if [[ -d "$newDirName" ]] ; then
        # Move files in dir to newDirName
        [[ $isVerbose = true ]] && echo "$newDirName already exists. Moving files in $dir to $newDirName" && echo
        files="$dir/*"
        mv $files "$newDirName"

        # Delete original dir
        [[ $isVerbose = true ]] && echo "Deleting $dir" && echo
        rmdir "$dir"

    # newDirName doesn't exist, rename dir
    else
        [[ $isVerbose = true ]] && echo "Renaming $dir to $newDirName" && echo
        mv "$dir" "$newDirName"
    fi

    # Remove spaces and unzip files as needed
    [[ $isVerbose = true ]] && echo "Removing spaces from files." && echo
    files="$newDirName/*"
    for file in $files
    do
        newFileName=$(removeSpaces "$file")
        # If newFileName is different than original
        if [[ ! -f "$newFileName" ]] ; then
            mv "$file" "$newFileName"
        fi
        
        # If file is a zip, unzip it and remove zip
        if [[ $newFileName = *.zip || $newFileName = *.ZIP ]] ; then
            [[ $isVerbose = true ]] && echo "Unzipping $newFileName" && echo
            unzip -q "$newFileName" -d "$newDirName" # -q suppresses unzip output
            rm "$newFileName"
        fi
    done

    IFS=defaultIFS # Reset IFS
done

# Delete original zip
[[ $isVerbose = true ]] && echo "Deleting $inputFile" && echo
rm "$inputFile"

[[ $isVerbose = true ]] && echo "Done."