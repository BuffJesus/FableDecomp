#include <cstdio>

struct Counted {
    void** vtbl;
    long   refs;
};

struct CIVCountedPointer {
    Counted* p;
    void release() {
        Counted* c = p;
        if (c) {
            if (--c->refs == 0) {
                ((void (__fastcall*)(Counted*))(c->vtbl[1]))(c);
            }
            p = 0;
        }
    }
};

void __fastcall Dest_val(CIVCountedPointer* self) {
    self->release();
}

static int g_freed = 0;
void __fastcall fakeDtor(Counted*) { g_freed = 1; }

int main() {
    void* vt[2] = { 0, (void*)&fakeDtor };
    Counted c; c.vtbl = vt; c.refs = 1;
    CIVCountedPointer cp; cp.p = &c;
    Dest_val(&cp);
    printf("PASS freed=%d ptr=%d\n", g_freed, cp.p == 0 ? 1 : 0);
    return 0;
}