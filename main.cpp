#include <iostream>
#include "memorypool.h"
#include <chrono>

using namespace std;

int main() {
    chrono::steady_clock::time_point start =chrono::steady_clock::now();

    Thing* t[1000];

    for (int i=0 ; i<1000; i++) // Lets have 1000x1000 allocations and deallocations to see some substantial difference in runtime.
    {
        for (int j = 0; j  <  1000; j++) {
               t[j] = new Thing();
             }
        for (int j = 0; j  <  1000; j++) {
             delete t[j];
             t[j] = NULL;       // Sets the now dangling pointer to NULL.
             }
    }

    chrono::steady_clock::time_point end = chrono::steady_clock::now();

    cout << "Allocation and deallocation time: "
                  << chrono::duration_cast<chrono::milliseconds>(end - start).count()
                  << "ms" <<endl;

    return 0;
}
