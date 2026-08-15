#include <cstdio>

struct CGraphicDataBank {
    void* vtbl;
    char  pad[0x10 - 0x04];
    void* p10;
    void* p14;
};

static void* g_freed[4];
static int g_n = 0;
extern "C" void FreeThing(void* p) { g_freed[g_n++] = p; }

#define VTBL_ADDR ((void*)0x12b3820)

void ChangeScene(CGraphicDataBank* self)
{
    self->vtbl = VTBL_ADDR;
    FreeThing(self->p14);
    FreeThing(self->p10);
}

int main() {
    CGraphicDataBank o;
    o.vtbl = (void*)0x1;
    o.p10 = (void*)0xAAAA;
    o.p14 = (void*)0xBBBB;
    ChangeScene(&o);
    bool ok = (g_n == 2)
           && (g_freed[0] == (void*)0xBBBB)   // p14 freed first
           && (g_freed[1] == (void*)0xAAAA)   // p10 freed second
           && (o.vtbl == VTBL_ADDR);
    if (ok) printf("CHANGESCENE_OK\n");
    else printf("FAIL n=%d\n", g_n);
    return 0;
}