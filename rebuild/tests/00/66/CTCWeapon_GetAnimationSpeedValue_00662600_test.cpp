#include <stdio.h>

struct RefObj {
    void** vtbl;
    int    ref;
    char   pad[0x64 - 8];
    float  value;
};
struct CTCWeapon {
    char   pad[0x70];
    RefObj* obj;
};

static int g_dtor_called = 0;
static void __fastcall FakeDtor(RefObj* p) { g_dtor_called++; }

struct Ptr {
    RefObj* p;
    Ptr(RefObj* q) : p(q) { if (p) ++p->ref; }
    ~Ptr() {
        if (--p->ref == 0) {
            typedef void (__fastcall *Fn)(RefObj*);
            ((Fn)(p->vtbl[1]))(p);
        }
    }
};

float __fastcall GetAnimationSpeedValue(CTCWeapon* self) {
    Ptr h(self->obj);
    return h.p->value;
}

int main() {
    void* vt[2];
    vt[0] = 0;
    vt[1] = (void*)&FakeDtor;

    RefObj o;
    o.vtbl = (void**)vt;
    o.ref = 1;              // held elsewhere -> won't reach 0
    o.value = 3.5f;

    CTCWeapon w;
    w.obj = &o;

    float r = GetAnimationSpeedValue(&w);
    // ref back to 1, dtor NOT called
    if (r == 3.5f && o.ref == 1 && g_dtor_called == 0) {
        // now a case where release hits zero
        RefObj o2;
        o2.vtbl = (void**)vt;
        o2.ref = 0;         // AddRef->1, read, Release->0 -> dtor
        o2.value = 7.25f;
        CTCWeapon w2;
        w2.obj = &o2;
        float r2 = GetAnimationSpeedValue(&w2);
        if (r2 == 7.25f && g_dtor_called == 1) {
            printf("PARITY_OK\n");
            return 0;
        }
    }
    printf("FAIL\n");
    return 1;
}