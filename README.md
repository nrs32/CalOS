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
