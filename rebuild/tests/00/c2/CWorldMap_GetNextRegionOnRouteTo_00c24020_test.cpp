#include <stdio.h>

struct CWorldMap {
    char pad0[8];
    long count;   // +0x08
    char pad1[8];
    long* arr;    // +0x14
};

static int g_helper_called = 0;
void __fastcall CWorldMap_helper(CWorldMap* self) { g_helper_called++; (void)self; }

long __fastcall CWorldMap_GetNextRegionOnRouteTo(CWorldMap* self)
{
    long i = self->count;
    if (i == 0)
        return 0;
    CWorldMap_helper(self);
    long r = self->arr[i];
    self->count = self->count - 1;
    return r;
}

int main()
{
    long data[8] = {100,101,102,103,104,105,106,107};
    CWorldMap m;
    m.count = 5;
    m.arr = data;

    // count==5 -> helper called, returns arr[5]=105, count becomes 4
    long v = CWorldMap_GetNextRegionOnRouteTo(&m);
    bool ok = (v == 105) && (m.count == 4) && (g_helper_called == 1);

    // exhaust
    v = CWorldMap_GetNextRegionOnRouteTo(&m); ok = ok && (v == 104) && (m.count == 3);

    // count==0 short-circuit: returns 0, helper NOT called
    m.count = 0;
    int before = g_helper_called;
    v = CWorldMap_GetNextRegionOnRouteTo(&m);
    ok = ok && (v == 0) && (g_helper_called == before);

    if (ok) printf("PARITY_OK\n");
    else printf("FAIL v=%ld count=%ld\n", v, m.count);
    return 0;
}