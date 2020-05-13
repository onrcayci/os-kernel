# Memory Management

This is the third and last phase of the kernel design. In this phase, memory management is added to the process management in order to provide a more realistic program execution. RAM is shortened from a string array of 1000 indeces to a string array of 40 indeces. Most of the process management design stayed the same except a new structure called **Memory Manager** has been added to the design which handles all of the paging related tasks.

RAM has been divided into **frames** and the programs are into **pages**. Program scripts are first copied into the Backing Store from the disk and then these copies are used to load the pages into the RAM using a **loader** method in the memory manager structure. The first two pages of a program is loaded to the RAM in the initial launch setup. If the program is longer than 2 pages, a page fault will occur after the execution of the initial 2 pages. Scheduler is modified to handle this page fault.

There are also methods to handle page faults when the RAM is full. Since this requires emptying a frame of the RAM, a method to choose this frame has been implemented. It randomly picks a frame that does not belong to the program that has the CPU and returns the starting index of this frame. Then, scheduler uploads its next paage to the RAM into the that frame and program execution continues.

Task switching and completion of a program are handled the same, except extra steps for end-of-frame exceptions are added. Overall, this last design brings together a simple I/O, parser, interpreter, process and memory manager together.
