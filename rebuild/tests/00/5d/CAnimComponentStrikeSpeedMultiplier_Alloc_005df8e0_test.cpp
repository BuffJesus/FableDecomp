#include <stdio.h>
#include <stdlib.h>

typedef unsigned long ULONG;

struct Obj { ULONG vtable; ULONG field4; ULONG field8; };

static void * my_new(unsigned int size) { return malloc(size); }

static Obj * Alloc()
{
    Obj * p = (Obj *)my_new(0xc);
    if (p) {
        p->field4 = 1;
        p->vtable = 0x1255800;
        return p;
    }
    return 0;
}

int main()
{
    Obj * o = Alloc();
    int ok = (o != 0) && (o->field4 == 1) && (o->vtable == 0x1255800);
    if (ok) printf("BEHAVIOR_PASS\n");
    else printf("BEHAVIOR_FAIL\n");
    if (o) free(o);
    return 0;
}