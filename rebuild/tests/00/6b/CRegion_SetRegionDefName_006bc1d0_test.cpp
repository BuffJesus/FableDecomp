#include <cstdio>

static int g_assigned = 0;
static int g_count_ret = 0;
static int g_applied = 0;

struct NameField {
    int v;
    void Assign(int name) { g_assigned = name; }
};

struct Counted {
    int GetCount() { return g_count_ret; }
};

struct Applied {
    void Apply() { g_applied = 1; }
};

static Counted g_counted;
static Applied g_applied_obj;

extern "C" Counted* __stdcall Resolve(NameField* p) { (void)p; return &g_counted; }
extern "C" Applied* __stdcall Resolve2(int count, void* p) { (void)count; (void)p; return &g_applied_obj; }

struct CRegion {
    char pad0[0x20];
    NameField field20;
    int field24;
};

void __fastcall CRegion_SetRegionDefName(CRegion* self, int, int name)
{
    self->field20.Assign(name);
    Counted* c = Resolve(&self->field20);
    int cnt = c->GetCount();
    if (cnt > 0) {
        Applied* a = Resolve2(cnt, &self->field24);
        a->Apply();
    }
}

int main() {
    CRegion rgn;
    rgn.field20.v = 0;
    rgn.field24 = 0;

    g_assigned = -1; g_applied = 0; g_count_ret = 5;
    CRegion_SetRegionDefName(&rgn, 0, 1234);
    if (g_assigned != 1234) { std::printf("FAIL assign\n"); return 1; }
    if (g_applied != 1) { std::printf("FAIL apply-positive\n"); return 1; }

    g_applied = 0; g_count_ret = 0;
    CRegion_SetRegionDefName(&rgn, 0, 7);
    if (g_applied != 0) { std::printf("FAIL apply-zero\n"); return 1; }

    g_applied = 0; g_count_ret = -3;
    CRegion_SetRegionDefName(&rgn, 0, 7);
    if (g_applied != 0) { std::printf("FAIL apply-neg\n"); return 1; }

    std::printf("CRegion_006bc1d0_TEST PASS\n");
    return 0;
}