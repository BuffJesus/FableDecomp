#include <cstdio>

struct CObj;
struct CObjVtbl {
    void (__fastcall *slots[32])(CObj*);
};
struct CObj {
    CObjVtbl* vtbl;
};
struct CTCCreatureNavigation {
    char pad0[0x0c];
    CObj* obj;
    char pad1[0x55 - 0x10];
    unsigned char flag;
};

static int g_called = 0;
static void __fastcall my_slot(CObj* o) { (void)o; g_called++; }

void __fastcall CTCCreatureNavigation_SetAsAbleToMoveThroughSolidObjects(CTCCreatureNavigation* self, void* edx, unsigned char b)
{
    if (b != self->flag) {
        CObj* o = self->obj;
        o->vtbl->slots[0x48/4](o);
    }
    self->flag = b;
}

int main()
{
    CObjVtbl vt;
    for (int i = 0; i < 32; ++i) vt.slots[i] = 0;
    vt.slots[0x48/4] = my_slot;
    CObj obj; obj.vtbl = &vt;
    CTCCreatureNavigation nav;
    nav.obj = &obj;
    nav.flag = 0;

    // same value -> no call
    CTCCreatureNavigation_SetAsAbleToMoveThroughSolidObjects(&nav, 0, 0);
    if (g_called != 0 || nav.flag != 0) { std::printf("FAIL a\n"); return 1; }

    // change value -> call + store
    CTCCreatureNavigation_SetAsAbleToMoveThroughSolidObjects(&nav, 0, 1);
    if (g_called != 1 || nav.flag != 1) { std::printf("FAIL b\n"); return 1; }

    // same again -> no call
    CTCCreatureNavigation_SetAsAbleToMoveThroughSolidObjects(&nav, 0, 1);
    if (g_called != 1 || nav.flag != 1) { std::printf("FAIL c\n"); return 1; }

    std::printf("CTCCreatureNavigation_006cb9d0_TEST PASS\n");
    return 0;
}