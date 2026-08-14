#include <cstdio>

struct Counted {
    void** vtable;
    int    refcount;
};

struct CIVCountedPointer_CTriangleList___ {
    Counted* ptr;
};

static int g_destroyed = 0;

void __fastcall fake_destroy(Counted* c) {
    (void)c;
    g_destroyed++;
}

void __fastcall Dest_val_CTriangleList___(CIVCountedPointer_CTriangleList___* self)
{
    Counted* c = self->ptr;
    if (c) {
        if (--c->refcount == 0) {
            typedef void (__fastcall *pfn)(Counted*);
            ((pfn)c->vtable[1])(c);
        }
        self->ptr = 0;
    }
}

int main() {
    CIVCountedPointer_CTriangleList___ p1;
    p1.ptr = 0;
    Dest_val_CTriangleList___(&p1);
    if (p1.ptr != 0 || g_destroyed != 0) { printf("FAIL1\n"); return 1; }

    void* vt[2];
    vt[0] = 0;
    vt[1] = (void*)&fake_destroy;

    Counted c2; c2.vtable = vt; c2.refcount = 2;
    CIVCountedPointer_CTriangleList___ p2; p2.ptr = &c2;
    Dest_val_CTriangleList___(&p2);
    if (c2.refcount != 1 || g_destroyed != 0 || p2.ptr != 0) { printf("FAIL2\n"); return 1; }

    Counted c3; c3.vtable = vt; c3.refcount = 1;
    CIVCountedPointer_CTriangleList___ p3; p3.ptr = &c3;
    Dest_val_CTriangleList___(&p3);
    if (c3.refcount != 0 || g_destroyed != 1 || p3.ptr != 0) { printf("FAIL3\n"); return 1; }

    printf("PARITY_OK\n");
    return 0;
}