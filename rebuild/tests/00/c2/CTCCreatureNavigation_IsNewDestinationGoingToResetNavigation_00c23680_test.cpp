#include <cstdio>

struct C3DVector { float x, y, z; };
struct CTCCreatureNavigation;

typedef bool (__stdcall *SlotFn)(CTCCreatureNavigation* nav, C3DVector* dest);

struct Helper {
    SlotFn* vtbl;
};

struct CTCCreatureNavigation {
    void* pad0;
    Helper* helper;
};

static CTCCreatureNavigation* g_seenNav;
static C3DVector* g_seenDest;

static bool __stdcall retTrue(CTCCreatureNavigation* nav, C3DVector* dest) {
    g_seenNav = nav; g_seenDest = dest; return true;
}
static bool __stdcall retFalse(CTCCreatureNavigation* nav, C3DVector* dest) {
    return false;
}

static bool model(CTCCreatureNavigation* self, C3DVector* dest, float f) {
    (void)f;
    return self->helper->vtbl[7](self, dest);
}

int main() {
    C3DVector d = {1.0f, 2.0f, 3.0f};
    SlotFn tblT[16]; for (int i=0;i<16;i++) tblT[i]=0; tblT[7]=retTrue;
    Helper hT; hT.vtbl = tblT;
    CTCCreatureNavigation navT; navT.pad0 = 0; navT.helper = &hT;
    g_seenNav = 0; g_seenDest = 0;
    bool r1 = model(&navT, &d, 5.0f);
    if (!r1) { printf("FAIL r1\n"); return 1; }
    if (g_seenNav != &navT) { printf("FAIL nav\n"); return 1; }
    if (g_seenDest != &d) { printf("FAIL dest\n"); return 1; }

    SlotFn tblF[16]; for (int i=0;i<16;i++) tblF[i]=0; tblF[7]=retFalse;
    Helper hF; hF.vtbl = tblF;
    CTCCreatureNavigation navF; navF.pad0 = 0; navF.helper = &hF;
    bool r2 = model(&navF, &d, 0.0f);
    if (r2) { printf("FAIL r2\n"); return 1; }

    printf("OK_00c23680\n");
    return 0;
}