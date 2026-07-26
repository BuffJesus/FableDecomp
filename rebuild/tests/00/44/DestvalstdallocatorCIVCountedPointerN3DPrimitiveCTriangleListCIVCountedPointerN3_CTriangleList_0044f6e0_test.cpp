#include <cstdio>

struct Counted { void* vtbl; long refs; };
struct CIVCountedPointer { Counted* p; };

static int g_released = 0;

void __fastcall Dest_val(CIVCountedPointer* self)
{
    Counted* c = self->p;
    if (c) {
        if (--c->refs == 0) {
            (*(void (__fastcall**)(Counted*))((char*)c->vtbl + 4))(c);
        }
        self->p = 0;
    }
}

void __fastcall doRelease(Counted* c) { g_released++; }

int main()
{
    void* vt[2];
    vt[0] = 0;
    vt[1] = (void*)&doRelease;

    /* case 1: null pointer -> no crash, stays null */
    CIVCountedPointer a; a.p = 0;
    Dest_val(&a);
    if (a.p != 0) { std::printf("FAIL null\n"); return 1; }

    /* case 2: refs=2 -> decremented to 1, no release, cleared */
    Counted c2; c2.vtbl = vt; c2.refs = 2;
    CIVCountedPointer b; b.p = &c2;
    Dest_val(&b);
    if (c2.refs != 1 || b.p != 0 || g_released != 0) { std::printf("FAIL refs2\n"); return 1; }

    /* case 3: refs=1 -> decremented to 0, release called, cleared */
    Counted c3; c3.vtbl = vt; c3.refs = 1;
    CIVCountedPointer d; d.p = &c3;
    Dest_val(&d);
    if (c3.refs != 0 || d.p != 0 || g_released != 1) { std::printf("FAIL refs1\n"); return 1; }

    std::printf("DestVal_0044f6e0_TEST PASS\n");
    return 0;
}