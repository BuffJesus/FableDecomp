#include <stdio.h>
#include <string.h>

struct C3DGeneratorObject {
    unsigned long fields[0x322];
};

static int g_called = 0;
static C3DGeneratorObject *g_arg = 0;

static void __cdecl ctorHelper(C3DGeneratorObject *p) {
    g_called = 1;
    g_arg = p;
}

void (__cdecl *g_ctorHelper)(C3DGeneratorObject *) = ctorHelper;

void __fastcall Cons_val(C3DGeneratorObject *self)
{
    if (self) {
        unsigned long *p = (unsigned long *)self;
        for (int i = 0; i < 0x322; ++i)
            p[i] = 0;
        g_ctorHelper(self);
    }
}

int main() {
    // null path: no crash, no call
    Cons_val(0);
    if (g_called != 0) { printf("FAIL null called\n"); return 1; }

    // non-null: zeroed + helper invoked with same ptr
    static C3DGeneratorObject obj;
    memset(&obj, 0xAB, sizeof(obj));
    Cons_val(&obj);
    if (!g_called) { printf("FAIL not called\n"); return 1; }
    if (g_arg != &obj) { printf("FAIL wrong arg\n"); return 1; }
    for (int i = 0; i < 0x322; ++i) {
        if (obj.fields[i] != 0) { printf("FAIL not zeroed at %d\n", i); return 1; }
    }
    printf("PARITY_OK_C9EC00\n");
    return 0;
}