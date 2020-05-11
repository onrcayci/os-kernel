# Simple Shell
This is the first phase of the design. This design includes a simple command line tool and a shell memory where a variable can be stored with its value. Following are the commands availabe in this design phase:

- help: Displays all commands
- quit: Terminates the shell with "Bye!"
- set VAR STRING: Stores a variable with a value to the shell memory
- print VAR: Displays the value of the stored variable
- run SCRIPT.TXT: Executes the file SCRIPT.TXT

This functionality is achieved by adding a parser and an interpreter method. Parser method breaks down the command into individual words and calls the interpreter method. Interpreter assumes the Linux command string. i.e. CMD SWITCH ARGS. Interpreter tries to match the CMD with the implemented functions.

Shell memory functionality is achieved by a string matrix. This is a 1000x2 matrix, and variable and its value are stored in the same row.