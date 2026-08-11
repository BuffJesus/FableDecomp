#include <cstdio>

struct Inner {
    char pad[0x78];
    int* array;   // +0x78
};

struct CTCMultiStrike {
    char pad0[0x0c];
    Inner* inner;   // +0x0c
    int index;      // +0x10
    char pad1[0x08];
    int current;    // +0x1c
    bool IsLastHit();
};

bool CTCMultiStrike::IsLastHit()
{
    return this->current == (this->inner->array[this->index] - 1);
}

int main()
{
    int arr[4] = { 1, 5, 10, 3 };
    Inner innerObj;
    innerObj.array = arr;

    CTCMultiStrike ms;
    Inner* innerPtr = &innerObj;
    ms.inner = innerPtr;

    // arr[2]=10, 10-1=9, current=9 -> true
    ms.index = 2;
    ms.current = 9;
    bool r1 = ms.IsLastHit();

    // arr[1]=5, 5-1=4, current=2 -> false
    ms.index = 1;
    ms.current = 2;
    bool r2 = ms.IsLastHit();

    // arr[0]=1, 1-1=0, current=0 -> true
    ms.index = 0;
    ms.current = 0;
    bool r3 = ms.IsLastHit();

    if (r1 && !r2 && r3) {
        printf("MULTISTRIKE_LASTHIT_OK\n");
        return 0;
    }
    printf("FAIL r1=%d r2=%d r3=%d\n", r1, r2, r3);
    return 1;
}