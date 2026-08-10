#include <stdio.h>
#include <string.h>

static void* g_vt = (void*)0x12b9578;
static int g_base_called = 0;
static void* g_base_self = 0;

struct Obj { void* vptr; };

static void BaseDtorModel(Obj* self){ g_base_called = 1; g_base_self = self; }

static void DtorModel(Obj* self){
    self->vptr = g_vt;
    BaseDtorModel(self);
}

int main(){
    Obj o; o.vptr = 0;
    DtorModel(&o);
    int ok = 1;
    if (o.vptr != g_vt) ok = 0;
    if (!g_base_called) ok = 0;
    if (g_base_self != (void*)&o) ok = 0;
    if (ok) printf("OK_00c69680\n");
    else printf("FAIL\n");
    return ok?0:1;
}