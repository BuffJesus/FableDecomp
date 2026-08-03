#include <stdio.h>

struct RcVtbl {
    void (__fastcall *dtor)(void *self);
    void (__fastcall *release_dtor)(void *self);
};
struct RcObj {
    RcVtbl *vtbl;   // +0x00
    long    refs;   // +0x04
    char    payload[0x40];
};
struct CTCInventoryItem {
    char   pad[0x0c];
    RcObj *m_holder; // +0x0c
};

// ---- function under test (copy of source_cpp) ----
struct CEngineGraphic;
CEngineGraphic * __fastcall GetEngineGraphic(CTCInventoryItem *self)
{
    RcObj *p = self->m_holder;
    if (p)
        ++p->refs;
    CEngineGraphic *ret = (CEngineGraphic *)((char *)p + 0x25);
    if (p) {
        if (--p->refs == 0)
            p->vtbl->release_dtor(p);
    }
    return ret;
}

static int g_dtor_called = 0;
void __fastcall stub_dtor(void *self) { g_dtor_called++; }

int main()
{
    RcVtbl vt; vt.dtor = 0; vt.release_dtor = stub_dtor;
    RcObj obj; obj.vtbl = &vt; obj.refs = 5;
    for (int i=0;i<(int)sizeof(obj.payload);++i) obj.payload[i]=(char)(i);

    CTCInventoryItem item;
    item.m_holder = &obj;

    // invoke __fastcall via ecx shim
    void *result = 0;
    CTCInventoryItem *pitem = &item;
    __asm {
        mov  ecx, pitem
        call GetEngineGraphic
        mov  result, eax
    }

    // expected: holder + 0x25
    void *expected = (char *)&obj + 0x25;
    if (result != expected) { printf("FAIL result mismatch %p vs %p\n", result, expected); return 1; }
    // refs balanced (AddRef then Release), non-zero so no dtor
    if (obj.refs != 5) { printf("FAIL refs=%ld\n", obj.refs); return 1; }
    if (g_dtor_called != 0) { printf("FAIL dtor called\n"); return 1; }

    // null holder path: returns 0 + 0x25 = 0x25
    CTCInventoryItem item2; item2.m_holder = 0;
    void *r2 = 0; CTCInventoryItem *p2 = &item2;
    __asm {
        mov  ecx, p2
        call GetEngineGraphic
        mov  r2, eax
    }
    if (r2 != (void*)0x25) { printf("FAIL null path %p\n", r2); return 1; }

    printf("OK_0x005d92d0\n");
    return 0;
}