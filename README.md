C Compiler

The compiled file is defined in main.c. The compiler will compile test.c by default.

#installation guide for c compiler:

sudo apt install gcc
sudo apt install make
sudo apt install gcc-multilib g++-multilib
sudo apt install nasm

#debugging compiler source code during compilation
sudo apt install gdb 

#make commands
make 
make clean

#debugging binary produced by compiler

gdb ./test
break main
layout asm