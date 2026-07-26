#include <cstdio>

struct CInner {
    void** vt;
    int captured;
};

struct CScriptThing {
    void* pad0;
    CInner* inner;
};

typedef void (__fastcall *SlotFn)(CInner*, int, int);

static CInner* g_last = 0;
static int g_arg = 0;

void __fastcall slot66(CInner* p, int edx, int arg)
{
    (void)edx;
    g_last = p;
    g_arg = arg;
    p->captured = arg;
}

void __fastcall CScriptThing_SetAsUsable(CScriptThing* self, int edx, int arg)
{
    CInner* p = self->inner;
    if (p) {
        void** vt = *(void***)p;
        ((SlotFn)vt[66])(p, edx, arg);
    }
}

int main()
{
    void* vtbl[80];
    for (int i = 0; i < 80; i++) vtbl[i] = 0;
    vtbl[66] = (void*)&slot66;

    CInner inner;
    inner.vt = vtbl;
    inner.captured = -1;

    CScriptThing t;
    t.pad0 = 0;
    t.inner = &inner;

    CScriptThing_SetAsUsable(&t, 0, 1234);
    if (g_last != &inner || g_arg != 1234 || inner.captured != 1234) {
        std::printf("FAIL: call path wrong\n");
        return 1;
    }

    /* null inner path: must not crash */
    CScriptThing t2;
    t2.pad0 = 0;
    t2.inner = 0;
    g_last = 0; g_arg = 0;
    CScriptThing_SetAsUsable(&t2, 0, 999);
    if (g_last != 0 || g_arg != 0) {
        std::printf("FAIL: null path called slot\n");
        return 1;
    }

    std::printf("CScriptThing_004ab040_TEST PASS\n");
    return 0;
}