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
        void expandPool();
        void cleanUp();
        heapList* headNode;
    public:
        MemoryPool(){
            headNode = nullptr;
            expandPool();
        }
        virtual ~MemoryPool(){
            cleanUp();
        }
        void* allocate(size_t);
        void free(void*);
};

MemoryPool myMemory;

// This Thing has operators new and delete overridden in order to use our custom MemoryPool
class Thing {
    private:
    std::string ThingName;
    int    ThingValue;
    
    public:
    Thing()
        : ThingName("Happy Thing"), ThingValue(0) {};
    void* operator new(size_t);
    void operator delete(void*);
};

void* Thing::operator new(size_t size){
    return myMemory.allocate(size);
}

void Thing::operator delete(void* pointerToDelete){
    myMemory.free(pointerToDelete);
}

void* MemoryPool::allocate(size_t size) {
    if(headNode == nullptr)
        expandPool();

    heapList* head = headNode;
    headNode = head->next;
    return head;
}

void MemoryPool::free(void *deleted) {
    heapList* head = static_cast<heapList*> (deleted);

    head->next = headNode;
    headNode = head;
}

void MemoryPool::expandPool() {
    size_t  size = (sizeof(Thing) > sizeof(heapList*)) ? sizeof(Thing) : sizeof(heapList*);

    heapList* head = reinterpret_cast<heapList*> (new char[size]);
    headNode = head;

    for(int i = 0; i < POOLSIZE; i++){
        head->next = reinterpret_cast<heapList*>(new char[size]);
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
