#include <stdio.h>

extern "C" void* __cdecl zcalloc(void* opaque, unsigned int items, unsigned int size);

static volatile unsigned int lastSize;
static volatile unsigned int calls;
extern "C" void* __cdecl testMalloc(unsigned int bytes)
{
    lastSize = bytes;
    ++calls;
    return reinterpret_cast<void*>(bytes ^ 0x5a5a5a5aU);
}
extern "C" void* (__cdecl* _imp__malloc)(unsigned int) = &testMalloc;

int main()
{
    const unsigned int values[8] = {0, 1, 2, 3, 0x10000U, 0x80000000U, 0xffffffffU, 0x12345678U};
    int cases = 0;
    for (int i = 0; i != 8; ++i) {
        for (int j = 0; j != 8; ++j) {
            unsigned int expected = values[i] * values[j];
            void* result = zcalloc(reinterpret_cast<void*>(0x1000 + i), values[i], values[j]);
            if (lastSize != expected || calls != static_cast<unsigned int>(cases + 1) ||
                result != reinterpret_cast<void*>(expected ^ 0x5a5a5a5aU)) {
                printf("FAIL i=%d j=%d size=%u expected=%u calls=%u result=%p callback=%p\n",
                       i, j, lastSize, expected, calls, result, reinterpret_cast<void*>(_imp__malloc));
                return 1;
            }
            ++cases;
        }
    }
    printf("ZCALLOC_PASS cases=%d\n", cases);
    return 0;
}
