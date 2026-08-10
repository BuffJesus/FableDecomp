#include <stdio.h>
#include <string.h>

struct Obj { void* vtbl; int touched; };

static int g_member_called = 0;
static int g_base_called = 0;

static void member_cleanup(Obj* o){ g_member_called++; o->touched = 1; }
static void base_dtor(Obj* o){ g_base_called++; o->touched = 2; }

// behavioural model of DtorIPM_E
static void model(Obj* o){
    *((unsigned int*)o) = 0x12a20f0u; // set vtable
    member_cleanup(o);
    base_dtor(o);
}

int main(){
    Obj o; o.vtbl = 0; o.touched = 0;
    model(&o);
    int ok = 1;
    if (*((unsigned int*)&o) != 0x12a20f0u) ok = 0;
    if (g_member_called != 1) ok = 0;
    if (g_base_called != 1) ok = 0;
    if (o.touched != 2) ok = 0;
    // second run to exercise counters incrementing
    Obj o2; o2.vtbl=(void*)0x5; o2.touched=99;
    model(&o2);
    if (g_member_called != 2) ok = 0;
    if (g_base_called != 2) ok = 0;
    if (ok) { printf("OK_00b495f0\n"); return 0; }
    printf("FAIL_00b495f0\n"); return 1;
}