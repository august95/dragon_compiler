C Compiler

The compiled file is defined in main.c. The compiler will compile test.c by default.  

#installation guide to compile the c compiler:

sudo apt install gcc
sudo apt install make
sudo apt install gcc-multilib g++-multilib #for 32 bit compatibility
sudo apt install nasm

#for debugging
sudo apt install gdb 

#make commands for building compiler
make 
make clean

#debugging binary produced by compiler
gdb ./test
#usefull commands 
    break main
    layout asm      #inspect the assembly instructions
    stepi           #when stepping through instruction use stepi not step
    print $eax      #it's possible to print content of registers