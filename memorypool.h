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

//A Custom memory manager, that improves performance by working on an assigned size of memory and thereby also reducing memory leaks.
Class MemoryPool
{
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

class Employee {
public:
// I know it is better to have the access identifier of these variables as private, lets work with this for now.
    string Name;
    string Position;

    void* operator new(size_t);
    void operator delete(void*);
};

void* Employee::operator new(size_t size){
    return myMemory.allocate(size);
}

void Employee::operator delete(void* pointerToDelete){
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
    size_t  size = (sizeof(Employee) > sizeof(heapList*)) ? sizeof(Employee) : sizeof(heapList*);

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
