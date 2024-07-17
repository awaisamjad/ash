#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int a = 5; // Stored in data section
int b[20]; // Stored in bss
const char* hello = "Hello world";
int main()
{ // Stored in text
    int x; // Stored in stack
    int* p = (int*)malloc(sizeof(int)); // Stored in heap
    printf("(Data)& a = %p\n", (void*)&a);
    printf("(Bss)&b[0] = 0x %lx\n", (unsigned long)&b[0]);
    printf("(Stack)& x = 0x %lx\n", (unsigned long)&x);
    printf("(Heap)p = 0x %lx\n", (unsigned long)p);
    printf("(ROData) “Hello” = 0x %s\n", hello);
    printf("(TEXT)main = 0x%lx\n", (unsigned long)main);
}
