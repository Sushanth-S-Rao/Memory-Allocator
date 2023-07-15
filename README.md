# Memory Allocator

## Description
This is a memory allocator program that dynamically allocates and deallocates the memory in heap on request. Much like how malloc() and free() work.

## Technology Stack
- C++: Memory Allocator is implemented in C++
- Dynamic Memory Allocation
- Memory Layout of C Program

## Functions Used
| *Function*                              | *Use* |
|:--------------------------------      |:-------------------------|
| struct Block{};                        | Structure of Memory Block |
| using word_t = intptr_t                | Machine specific word size |
| word_t *alloc(size_t size);            | Dynamic Allocator function |
| inline size_t align(size_t n);         | Alignment or Padding function |
| inline size_t allocSize(size_t size);  | Returns total allocation size |
| void free(word_t *data);               | Function to Free the mapped heap |
| Block *requestFromOS(size_t size);     | Function that requests the specified size in HEAP to be allocated using sbrk() call |
| Block *getHeader(word_t *data);        | Returns Header of user data |
| Block *findFreeBlock(size_t size);     | Dynamic Allocation through searching the available block | 


## Additional Documentation
To know more about the implementation of functions and working of program, refer the comments above the function. To understand about the Memory Allocation, Memory Layout of C, Dynamic Allocation as well implementation of this program, refer to the article I have written [here](https://medium.com/@sushanthsrao/memory-allocation-in-c-and-custom-memory-allocator-program-89fcd5e50c3)

## License
[MIT Licence](https://choosealicense.com/licenses/mit/#)
