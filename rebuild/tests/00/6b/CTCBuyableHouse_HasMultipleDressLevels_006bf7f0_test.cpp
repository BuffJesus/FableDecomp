#include <cstdio>

struct DressData {
    char pad0[0x28];
    int* first;
    int* last;
};

struct CTCBuyableHouse {
    char pad0[0xc];
    DressData* dress;
};

bool __fastcall HasMultipleDressLevels(CTCBuyableHouse* self);

int main()
{
    int arr[4] = {0,1,2,3};

    CTCBuyableHouse h;
    DressData d;

    // null dress -> false
    h.dress = 0;
    bool r0 = HasMultipleDressLevels(&h);

    // 0 elements -> false
    d.first = arr; d.last = arr;
    h.dress = &d;
    bool r1 = HasMultipleDressLevels(&h);

    // 1 element -> false
    d.first = arr; d.last = arr + 1;
    bool r2 = HasMultipleDressLevels(&h);

    // 2 elements -> true
    d.first = arr; d.last = arr + 2;
    bool r3 = HasMultipleDressLevels(&h);

    // 4 elements -> true
    d.first = arr; d.last = arr + 4;
    bool r4 = HasMultipleDressLevels(&h);

    if (!r0 && !r1 && !r2 && r3 && r4) {
        printf("PASS_HASMULTIPLEDRESSLEVELS\n");
        return 0;
    }
    printf("FAIL %d %d %d %d %d\n", r0, r1, r2, r3, r4);
    return 1;
}