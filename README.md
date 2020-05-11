# OS Kernel
This is a simple kernel built in C in order to practice operating systems design. There are three different phases of this kernel.

## Simple Shell
This is the first phase of the kernel design. It is a simple command line tool that uses a parser and an interpreter to implement custom shell commands. Following are the available commands for this tool:

- help: Displays all commands
- quit: Terminates the shell with "Bye!"
- set VAR STRING: Stores a variable with a value to the shell memory
- print VAR: Displays the value of the stored variable
- run SCRIPT.TXT: Executes the file SCRIPT.TXT