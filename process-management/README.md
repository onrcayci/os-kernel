# Process Management

This is the second phase of the kernel design. First phase was able to only run one script at a time using the **"run"** command. However, this phase brings the functionality of running up to 3 scripts at the same time using the **"exec"** command. This design includes new structures such as the CPU, RAM and PCB in order to implement the process management.

**RAM** is a seperate structure from the shell memory. This is a string array that is used to load the contents of the scripts. For now, all of the contents of the script is loaded onto the RAM instead of paging. **PCB** sctructure then keeps track of the starting and the ending indeces in the RAM of the loaded script. These indeces are used during runtime to determine the next line and whether the program has reached its end. Each script gets its own PCB during the execution of the **"exec"** command.

A **Ready Queue** is also implemented into the kernel design in order to simulate the task switching. Also, this is how an operating system is able to run more than one program at the same time. The ready queue in this design is FIFO (First In First Out) and Round-Robin. Lastly the **CPU** structure is used to simulate the runtime of the programs. It has a run command which executes for a given quanta.

A **Scheduler** is also designed as a method inside the kernel file in order to achieve the concurrent program execution. This is the main method that is responsible for task switching. Furthermore, when a program finishes its execution, it also frees up the RAM by removing the loaded commands of the program from the RAM and deleting its PCB.
