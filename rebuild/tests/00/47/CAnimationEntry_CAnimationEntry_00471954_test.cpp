#include <stdio.h>

struct RefCounted { int vt; int refcount; };

struct CAnimationEntryData {
    int a;
    int b;
    int c;
    RefCounted* p;
};

CAnimationEntryData* __fastcall CAnimationEntry(CAnimationEntryData* self, void* edx_, CAnimationEntryData* src)
{
    self->a = src->a;
    self->b = src->b;
    self->c = src->c;
    RefCounted* p = src->p;
    self->p = p;
    if (p)
        p->refcount++;
    return self;
}

int main()
{
    RefCounted rc; rc.vt = 0x1234; rc.refcount = 5;
    CAnimationEntryData src; src.a = 11; src.b = 22; src.c = 33; src.p = &rc;
    CAnimationEntryData dst; dst.a = 0; dst.b = 0; dst.c = 0; dst.p = 0;

    CAnimationEntryData* r = CAnimationEntry(&dst, 0, &src);

    int ok = 1;
    if (r != &dst) ok = 0;
    if (dst.a != 11 || dst.b != 22 || dst.c != 33) ok = 0;
    if (dst.p != &rc) ok = 0;
    if (rc.refcount != 6) ok = 0;

    CAnimationEntryData src2; src2.a = 1; src2.b = 2; src2.c = 3; src2.p = 0;
    CAnimationEntryData dst2; dst2.a = 9; dst2.b = 9; dst2.c = 9; dst2.p = (RefCounted*)0xdead;
    CAnimationEntry(&dst2, 0, &src2);
    if (dst2.a != 1 || dst2.b != 2 || dst2.c != 3 || dst2.p != 0) ok = 0;

    if (ok) { printf("OK_0x00471954\n"); return 0; }
    printf("FAIL\n"); return 1;
}