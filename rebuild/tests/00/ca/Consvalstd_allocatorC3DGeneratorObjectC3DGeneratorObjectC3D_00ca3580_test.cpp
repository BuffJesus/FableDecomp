#include <stdio.h>
#include <string.h>

struct C3DGeneratorObject { char raw[96]; };

static int g_called = 0;
static C3DGeneratorObject* g_last = 0;

static void my_ctor(C3DGeneratorObject* p) { g_called++; g_last = p; }

static void cons_val(C3DGeneratorObject* self, C3DGeneratorObject* /*last*/)
{
    C3DGeneratorObject* p = self;
    if (p) {
        for (int i = 0; i < 24; ++i)
            ((unsigned long*)p)[i] = 0;
        my_ctor(p);
    }
}

int main()
{
    C3DGeneratorObject obj;
    memset(&obj, 0xAA, sizeof(obj));

    cons_val(0, 0);
    if (g_called != 0) { printf("BAD null path\n"); return 1; }

    cons_val(&obj, &obj);
    if (g_called != 1) { printf("BAD not called once\n"); return 1; }
    if (g_last != &obj) { printf("BAD wrong ptr\n"); return 1; }
    for (int i = 0; i < 96; ++i)
        if (obj.raw[i] != 0) { printf("BAD not zeroed at %d\n", i); return 1; }

    printf("CONSVAL_OK\n");
    return 0;
}