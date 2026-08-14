#include <stdio.h>
#include <string.h>

struct C3DGeneratorObject { unsigned long m[0x84]; };

static int g_ctor_called = 0;
static C3DGeneratorObject* g_ctor_arg = 0;

static C3DGeneratorObject* ctor(C3DGeneratorObject* p) {
    g_ctor_called++; g_ctor_arg = p; return p;
}

// Behaviour model of _Cons_val: null-check, zero 0x84 dwords, call ctor(ptr).
static void cons_val(void* self) {
    C3DGeneratorObject* p = (C3DGeneratorObject*)self;
    if (p) {
        for (int i = 0; i < 0x84; ++i) ((unsigned long*)p)[i] = 0;
        ctor(p);
    }
}

int main() {
    C3DGeneratorObject obj;
    memset(&obj, 0xAB, sizeof(obj));

    cons_val(&obj);

    // all dwords zeroed
    for (int i = 0; i < 0x84; ++i) {
        if (obj.m[i] != 0) { printf("FAIL zero at %d\n", i); return 1; }
    }
    if (g_ctor_called != 1) { printf("FAIL ctor count %d\n", g_ctor_called); return 1; }
    if (g_ctor_arg != &obj) { printf("FAIL ctor arg\n"); return 1; }

    // null path: no ctor call, no crash
    g_ctor_called = 0;
    cons_val(0);
    if (g_ctor_called != 0) { printf("FAIL null path\n"); return 1; }

    printf("CONSVAL_OK\n");
    return 0;
}