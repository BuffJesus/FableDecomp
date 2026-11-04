#include <stdio.h>

extern "C" void __cdecl zcfree(void* opaque, void* pointer);

static volatile unsigned int calls;
static void* volatile lastPointer;
extern "C" void __cdecl testFree(void* pointer)
{
    lastPointer = pointer;
    ++calls;
}
extern "C" void (__cdecl* _imp__free)(void*) = &testFree;

int main()
{
    const unsigned long values[8] = {0, 1, 2, 0x10000UL, 0x80000000UL,
                                     0xffffffffUL, 0x12345678UL, 0x87654321UL};
    int cases = 0;
    for (int i = 0; i != 8; ++i) {
        for (int j = 0; j != 2; ++j) {
            void* pointer = reinterpret_cast<void*>(values[i]);
            zcfree(reinterpret_cast<void*>(values[(i + j + 1) % 8]), pointer);
            if (calls != static_cast<unsigned int>(cases + 1) || lastPointer != pointer) {
                printf("FAIL i=%d j=%d calls=%u got=%p expected=%p\n",
                       i, j, calls, lastPointer, pointer);
                return 1;
            }
            ++cases;
        }
    }
    printf("ZCFREE_PASS cases=%d\n", cases);
    return 0;
}
