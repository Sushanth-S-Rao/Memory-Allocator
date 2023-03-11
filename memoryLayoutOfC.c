#include<stdio.h>
#include<stdlib.h>

// 1) Text Segment: Contains instructions to be executed after compilation

// 2) Initialised Data Segment
int global_var = 50;
const int global_const_var = 100;
char *greet = "Hello There";

// 3) Uninitialised Data Segment (BSS)
int global_var_uninitialised;
int *nowhere;

int add(int a, int b);

int main(int argc, char *argv[]) {   // 6) Command Line Arguments
  // 4.1) Stack frame 1
  int local_var = 99;
  int *nowhere = &local_var;
  int answer = add(local_var, 1);    // 4) add function should return the result to answer. This is also stored in Stack
  printf("%d \n", answer);
  // 5) Heap
  int *dynamic_heap = (int*) malloc (10);

  return 0;
}

int add(int a, int b) {                   // 4.2) Stack frame 2 (for arguments a and b)
  static int static_var2 = 55;            // 2) Initialised Data Segment
  static int static_var_uninitialised2;   // 3) Uninitialised Data Segment (BSS)

  // 4.2) Stack frame 2
  int result;
  result = a + b;
  // 4.3) If the function `add` is called again (recursion), Stack Frame 3 is created which stores local variables again and so forth
  return result;
}
