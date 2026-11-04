#include <stdio.h>
#include <stdlib.h>
#include <new>
#include "fable_string.h"
#include "fable_small_vector.h"
static CCharString* expected[3];
static unsigned destroyed, freed;
static void* expectedAllocation;
CCharString::CCharString() : storage_(0) {}
CCharString::~CCharString()
{
    if (destroyed >= 3 || this != expected[destroyed]) {
        printf("unexpected destructor %p index %u\n", this, destroyed); exit(10);
    }
    ++destroyed;
}
void operator delete[](void* p)
{
    if (p != expectedAllocation || destroyed != 3) {
        printf("unexpected free %p expected %p destroyed %u\n", p, expectedAllocation, destroyed); exit(11);
    }
    ++freed;
    free(p);
}
int main()
{
    CSmallVector<CCharString, 8> strings;
    strings.Data = 0;
    strings.Size = 7;
    strings.Clear();
    if (strings.Data || strings.Size || destroyed || freed) return 1;
    strings.Data = new CCharString[3];
    expectedAllocation = reinterpret_cast<unsigned char*>(strings.Data) - 4;
    expected[0] = strings.Data + 2;
    expected[1] = strings.Data + 1;
    expected[2] = strings.Data;
    // Destruction uses the allocation cookie, not the logical length byte.
    strings.Size = 1;
    strings.Clear();
    if (strings.Data || strings.Size || destroyed != 3 || freed != 1) return 2;
    strings.Clear();
    if (destroyed != 3 || freed != 1) return 3;
    puts("SMALL_VECTOR_CLEAR_009d2110_PASS");
    return 0;
}
