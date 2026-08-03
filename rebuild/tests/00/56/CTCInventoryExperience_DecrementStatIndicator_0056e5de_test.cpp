#include <cstdio>

struct CTCInventoryExperience {
    char pad[0x1bc];
    int count;
    int* __fastcall DecrementStatIndicator();
};

int* __fastcall CTCInventoryExperience::DecrementStatIndicator()
{
    int* p = &this->count;
    int v = *p - 1;
    if (v >= 0)
        *p = v;
    return p;
}

static void call_it(CTCInventoryExperience* p)
{
    __asm { mov ecx, p }
    __asm { call CTCInventoryExperience::DecrementStatIndicator }
}

int main()
{
    CTCInventoryExperience o;
    o.count = 3;
    call_it(&o); // 3 -> 2
    if (o.count != 2) { printf("BAD1 %d\n", o.count); return 1; }
    call_it(&o); // 2 -> 1
    call_it(&o); // 1 -> 0
    if (o.count != 0) { printf("BAD2 %d\n", o.count); return 1; }
    call_it(&o); // 0 -> would be -1, clamp: stays 0
    if (o.count != 0) { printf("BAD3 %d\n", o.count); return 1; }
    o.count = -5;
    call_it(&o); // -5 -> -6 negative, no store, stays -5
    if (o.count != -5) { printf("BAD4 %d\n", o.count); return 1; }
    printf("OK_0x0056e5de count=%d\n", o.count);
    return 0;
}