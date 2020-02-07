### CalOS
Simple OS written in Python

#### run main
python main.py

### Batch Program Run
#### Load programs  
l \<addr\> \<fileName\> 
  
Example:  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; --> l 20 mult.asm  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; --> l 100 fib.asm  
  
#### Add list of program starting locations
D \<addrOfList\>  
Enter values in list  
  
Example:  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; --> D 130  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; --> 20  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; --> 100  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; --> 0  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; --> .  

#### Run the Program list
R \<addrOfList\>  
  
Example:  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; --> R 130
