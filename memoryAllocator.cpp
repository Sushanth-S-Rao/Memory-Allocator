#include <iostream>
#include <assert.h>
#include <unistd.h> 	// for "sbrk" call

// Machine word size. Depending on the architecture, it can be 4 (int) or 8 (long) bytes
using word_t = intptr_t;

// Functions Declared
word_t *alloc(size_t size);
inline size_t align(size_t n);
inline size_t allocSize(size_t size);
void free(word_t *data);

// Structure of block of memory. Contains the object header structure, and the actual payload pointer
struct Block {
	// 1) Object Header
	size_t size; 	// Block Size
  bool used; 		// Whether the block is currently used or not
  Block *next; 	// Next block in the list

	// 2) User data
  word_t data[1];		// Payload pointer
};

// We’ll track the start and the end of the heap
static Block *heapStart = nullptr;	// Heap start. Initialized on first allocation
static auto top = heapStart;				// Current top. Updated on each allocation

// Functions Declared
Block *requestFromOS(size_t size);
Block *getHeader(word_t *data);
Block *firstFit(size_t size);
Block *findFreeBlock(size_t size);


// Dynamic Allocation
// Allocates block of memory of (atleast) 'size' bytes
word_t *alloc(size_t size) {
	size = align(size);

	// 1) Search for available free block
	if(auto block = findFreeBlock(size))
		return block -> data;

	// 2) Request memory from the OS, if no free block is found
	auto block = requestFromOS(size);
	block -> size = size;
	block -> used = true;

	// init heap
	if(heapStart == nullptr)
		heapStart = block;

	// Chain the Blocks
	if(top != nullptr)
		top -> next = block;

	top = block;

	// User Payload
	return block -> data;
}


// Memory Alignment
// Aligns the size by the machine word
inline size_t align(size_t n) {
  return (n + sizeof(word_t) - 1) & ~(sizeof(word_t) - 1);
}


// Memory Mapping
// Returns total allocation size (header + first data word): Since word_t data[1] allocates one word inside Block, we subtract it from size request
inline size_t allocSize(size_t size) {
	return size + sizeof(Block) - sizeof(std::declval<Block>().data);
}

// Mapping memory from the OS
Block *requestFromOS(size_t size) {
	// 1) Current Heap Break
	auto block = (Block *)sbrk(0);

	// 2) Out Of Memory
	if(sbrk(allocSize(size)) == (void *) - 1)
		return nullptr;

	return block;
}


// Helper Function to obtain Object Header from User Pointer
Block *getHeader(word_t *data) {
	return (Block *)((char *)data + sizeof(std::declval<Block>().data) - sizeof(Block));
}


// Frees a previously allocated block
void free(word_t *data) {
  auto block = getHeader(data);
  block -> used = false;
}

// Search the linked list for available free block
Block *findFreeBlock(size_t size) {
	auto block = heapStart;
	while(block != nullptr) {
		if(block -> used || block -> size < size) {
			block = block -> next;
			continue;
		}
		// Block is Found
		return block;
	}
	return nullptr;
}


int main(int argc, char const *argv[]) {
  // Test case 1: Alignment
  auto p1 = alloc(3);
  auto p1b = getHeader(p1);
  assert(p1b->size == sizeof(word_t));
	std::cout << "Alignment 1 Successful\n";

  // Test case 2: Exact amount of aligned
  auto p2 = alloc(8);
  auto p2b = getHeader(p2);
  assert(p2b->size == 8);
  std::cout << "Alignment 2 Successful\n";

	// Test case 3: Free the object
	free(p2);
	assert(p2b->used == false);
	std::cout << "Freeing the Memory Block Successful\n";

	// Test case 4: The block is reused
	auto p3 = alloc(8);
	auto p3b = getHeader(p3);
	assert(p3b->size == 8);
	assert(p3b == p2b);
	std::cout << "Reusing the Memory Block Successful\n";

  return 1;
}
