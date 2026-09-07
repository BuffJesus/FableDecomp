#include <stdlib.h>
#include <stdio.h>

static int copied_first;
static int copied_last;
static void* copied_destination;

static void Copy(int first, int last, void* destination)
{
    copied_first = first;
    copied_last = last;
    copied_destination = destination;
}

static void* Allocate(int count, int first, int last)
{
    void* allocation = count != 0 ? malloc(count << 2) : 0;
    Copy(first, last, allocation);
    return allocation;
}

int main()
{
    void* allocation = Allocate(3, 11, 29);
    if (!allocation || copied_destination != allocation || copied_first != 11 || copied_last != 29) return 1;
    free(allocation);
    if (Allocate(0, 7, 7) != 0 || copied_destination != 0 || copied_first != 7 || copied_last != 7) return 2;
    puts("STL_ALLOCATE_ARRAY_PASS");
    return 0;
}