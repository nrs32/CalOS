## Concurrent Baking  
Run a bakery using threads and semophores for concurrency.   
  
There is one baker who bakes bread and checks out customers concurrently.   
Only 5 customers can be in the bakery at a time due to space and social distancing (COVID-19).  
Customers must wait to enter the bakery when it is full and wait for their bread when inside and reciept after paying.
  
Customers and baker print updates of their progress in the console.   
  
## Command Shell
SNShell is a command shell that reads a user command from the keyboard and performs that command on behalf of the user.  
  
Supported commands are: `ls`, `ps`, `pwd`, `cd` and `pwd`.  

Command prompt shows SNShell in magenta and current directory path in cyan.  
  
**Compile Program**  
`g++ -g -o snshell main.cpp SNShell.cpp Path.cpp Prompt.cpp Utils.cpp CommandLine.cpp`  

**Run Program**  
`./snshell`  
  
## Context Switching  
Improve CalOS to handle context switching based on time slice.   
  
Hande timer interruptions by changing the active process and updating the PCB record, and update the ready queue.  
  
**Run Program**  
`python3 main.py`  
  
**Run Program Example:**  
Examle run switching between fib and mult programs (also see CalOS).  
  
Toggle Print Progress  
--> `!`  
Load mult at location 20  
--> `l 20 mult.asm`  
Enter values for mult  
--> `d 10`    
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; --> `5`  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; --> `4`  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; --> `.`  
Load fib at location 100  
--> `l 100 fib.asm`  
Enter value for fib  
--> `d 50`  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; --> `8`  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; --> `.`  
Run it  
--> `r`  
  
## FileCopy  
Program in C to copy the contents of one file to another after argument verification.  
  
**Compile program**  
`gcc copy.c -o copy`  
  
**Run program**  
`./copy <srcFile> <destFile>`

## CalOS
Simple OS written in Python

**run main**  
`python main.py`

**Load programs**    
`l <addr> <fileName>` 
  
Example:  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; --> `l 20 mult.asm`  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; --> `l 100 fib.asm`  
  
If loading fib, also use:  
 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; --> `D 50`  
 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; --> `<numIterationsThroughFibonacciSequence>`  
 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; --> `.`  
  
**Add list of program starting locations**  
`D <addrOfList>`  (Then enter values of list.)  
  
Example:  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; --> `D 130`  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; --> `20`  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; --> `100`  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; --> `0`  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; --> `.`  

**Run the Program list**  
`R <addrOfList>`  
  
Example:  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; --> `R 130`
