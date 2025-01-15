## Getting started.

To read a file into stdin do:
    1) python3 lp_solver.py < name_of_file.txt
       python3 lp_solver.py < vanderbei_example3.6.txt
    2) Then press enter
in case python3 doesn't work type python instead. python3 should work 

## Extra features: 
Only one extra feature is implemented
The solver has Primal-Dual Methods implemented.

## Limitations
1) The solver has been tested on Ubuntu, linux Debian operating system. In particular, the "<" symbol, is reserved on the Window operating system.
2) The solver can only solve maximization and minimization problems, it does not solve linear programs with equality constraints.
3) **Note to outside reader** I was only able to unzip the file: test_LPs_volume1.tar.bz2 on linux Debian and Mac operating systems. That folder contains test linear program files. 