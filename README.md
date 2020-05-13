# OS Kernel

This is a simple kernel built in C in order to practice operating systems design. There are three different phases of this kernel.

## Simple Shell

This is the first phase of the kernel design. It is a simple command line tool that uses a parser and an interpreter to implement custom shell commands. Following are the available commands for this tool:

- help: Displays all commands
- quit: Terminates the shell with "Bye!"
- set VAR STRING: Stores a variable with a value to the shell memory
- print VAR: Displays the value of the stored variable
- run SCRIPT.TXT: Executes the file SCRIPT.TXT

## Process Management

This is the second phase of the kernel design. Here, a new command called "exec" has been added to the kernel. Additional structures such as ram, cpu, pcb and ready queue are also added to the design. These additions allow the design to mimic the process management of an operating system, especially when there are more than 1 program executing.

A scheduler method has also been added to the design in order to achieve that process management. This is the main method that controls the task switching and ready queue.

Following is the added command to the shell:

- exec: Executes concurrent programs. It can execute up to 3 programs.
  - How to call the command: exec SCRIPT1.TXT SCRIPT2.TXT

## Memory Management

This is the third and final phase of the kernel design. Following the process management, a memory management design is also added. This phase aims to mimic the RAM management in an operating system. Page and frame modeling is chosen for this design. The RAM is divided into frames, and pages from a program are loaded to these frames. CPU will run only these pages for a certain quanta and then task switch occurs. Page fault handling is also added to the method.

Furthermore, a backing store has also been added to design. Programs to be executed are copied into the backing store first, and these copies are used to load new pages into the RAM. PCB and RAM structures have been updated to support this new design, and a new memory manager has been designed in order to add the memory management functionality.
