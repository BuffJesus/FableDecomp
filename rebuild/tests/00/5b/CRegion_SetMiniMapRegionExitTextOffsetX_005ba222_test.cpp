#include <cstdio>

struct CRegion {
    void* f0;
};

extern "C" void* helper_4292c0(void* p);

static void* g_seen;
extern "C" void* helper_4292c0(void* p) {
    g_seen = p;
    return (void*)((char*)p + 0x100);
}

CRegion* __fastcall SetMiniMapRegionExitTextOffsetX(CRegion* self)
{
    self->f0 = helper_4292c0(self->f0);
    return self;
}

int main() {
    CRegion r;
    r.f0 = (void*)0x1000;
    CRegion* ret = SetMiniMapRegionExitTextOffsetX(&r);
    bool ok = (ret == &r)
           && (g_seen == (void*)0x1000)
           && (r.f0 == (void*)0x1100);
    if (ok) printf("005ba222_TEST PASS\n");
    else printf("FAIL ret=%p seen=%p f0=%p\n", (void*)ret, g_seen, r.f0);
    return 0;
}