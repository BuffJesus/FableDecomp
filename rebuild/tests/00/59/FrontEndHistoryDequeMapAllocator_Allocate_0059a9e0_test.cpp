#include <cstdio>
#include <malloc.h>
#include <stdlib.h>

// Declaration only: the call must bind to the instantiation in the source.
template<class T>
class FrontEndHistoryDequeMapAllocator
{
public:
    T* Allocate(unsigned long count, const void* hint);
};

int main()
{
    int failures = 0;
    FrontEndHistoryDequeMapAllocator<void*> allocator;
    int hint_a = 1;
    int hint_b = 2;

    if (allocator.Allocate(0, &hint_a) != 0)
    {
        std::printf("zero-count allocation failed\n");
        ++failures;
    }

    void** single = allocator.Allocate(1, &hint_a);
    if (single == 0 || _msize(single) != 4)
    {
        std::printf("single-slot allocation failed\n");
        ++failures;
    }

    void** multi = allocator.Allocate(7, 0);
    if (multi == 0 || multi == single || _msize(multi) != 28)
    {
        std::printf("multi-slot allocation/hint handling failed\n");
        ++failures;
    }
    else
    {
        for (int i = 0; i < 7; ++i)
        {
            multi[i] = &hint_b;
        }
        if (multi[6] != &hint_b)
        {
            std::printf("multi-slot storage failed\n");
            ++failures;
        }
    }

    free(single);
    free(multi);

    if (failures != 0)
    {
        std::printf(
            "FRONTEND_0059a9e0_TEST FAIL count=%d\n",
            failures);
        return 1;
    }

    std::printf("FRONTEND_0059a9e0_TEST PASS\n");
    return 0;
}
