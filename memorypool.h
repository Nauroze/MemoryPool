#ifndef MEMORYPOOL_H
#define MEMORYPOOL_H
#include <sys/types.h>
#include <string>
#define POOLSIZE 32

using namespace std;

//Linked List to store next memory location
struct heapList {
    heapList* next;
};

// A Custom memory manager, that improves performance by working on preallocated chunks of memory
// rather an incremental allocations to the heap. Memory pool size will keep increasing as it becomes required.
class MemoryPool
{
    private:
        void expandPool(size_t size = 0);
        void cleanUp();
        heapList* headNode;
    public:
        MemoryPool(){
            headNode = nullptr;
        }
        ~MemoryPool(){
            cleanUp();
        }
        void* allocate(size_t);
        void free(void*);
};

void* MemoryPool::allocate(size_t size) {
    if(headNode == nullptr)
        expandPool(size);

    heapList* head = headNode;
    headNode = head->next;
    return head;
}

void MemoryPool::free(void *deleted) {
    heapList* head = static_cast<heapList*> (deleted);

    head->next = headNode;
    headNode = head;
}

void MemoryPool::expandPool(size_t size) {
    size_t sizeofHeapList = sizeof(heapList*);
    size_t new_size = (size > sizeofHeapList) ? size : sizeofHeapList;

    heapList* head = reinterpret_cast<heapList*> (new char[new_size]);
    headNode = head;

    for(int i = 0; i < POOLSIZE; i++){
        head->next = reinterpret_cast<heapList*>(new char[new_size]);
        head = head->next;
    }

    head->next = nullptr;
}

void MemoryPool::cleanUp() {
    heapList* nextPtr = headNode;
    for(; nextPtr; nextPtr = headNode){
        headNode = headNode->next;
        delete[] nextPtr;
    }
}

#endif // MEMORYPOOL_H
