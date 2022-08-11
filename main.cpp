#include <iostream>
#include "memorypool.h"
#include <chrono>

using namespace std;

struct Thing
{
    int value;
    std::string str;

    Thing() : value(0), str("Happy Thing") {}
    
    void Print() {
        std::cout << str << " has a value of " << value << std::endl;
    }
    
    void* operator new(size_t size){
        return s_Memory.allocate(size);
    }

    void operator delete(void* pointerToDelete){
        s_Memory.free(pointerToDelete);
    }

    private:
    static MemoryPool s_Memory;
};

MemoryPool Thing::s_Memory;

int main() {
    chrono::steady_clock::time_point start =chrono::steady_clock::now();

    Thing* t;
    for (int i=0 ; i<1000; i++) // Lets have 1000x1000 allocations and deallocations to see some substantial difference in runtime.
    {
        for (int j = 0; j  <  1000; j++) {
               t = new Thing();
               delete t;
             }
    }

    chrono::steady_clock::time_point end = chrono::steady_clock::now();

    cout << "Allocation and deallocation time: "
                  << chrono::duration_cast<chrono::milliseconds>(end - start).count()
                  << "ms" <<endl;

    return 0;
}
