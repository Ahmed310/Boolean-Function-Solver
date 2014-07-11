Boolean-Function-Solver
=======================

In this project The Quine–McCluskey algorithm is implemented in C language.

MintermMinimizer folder contains the text file (input.txt) which
contains the input in binary format. This program solve minter upto
32 bit variable literal. the plus point of this application is that 
it display each step on screen.

How to use program:
Make sure that you have installed visual studio 2010, then
open MintermMinimizer.sln and run the program


Input format:

0001;

0001,

1111;


each line is end with ";" or "," and keep the size of input same
otherwise you got wrong or no answer.

if you mistakenly repeat some inputs then this program can take
care of this and correct this with out any effect on final answer 

0000;

0001;

0000;

0001;

1111;


Dont Care Minterms:
for dont care minterms write d or D at the end of literal
0000;

0001;

0001d;

1111d;




Video demo of this pregram :
link:
http://www.mediafire.com/watch/rzksb6h0q0yrmoz/Minterm_Minimizer_Demo.avi

Executable file:
http://www.mediafire.com/download/a3a9e96lll6baa8/Minterm_Solver(exe).zip
