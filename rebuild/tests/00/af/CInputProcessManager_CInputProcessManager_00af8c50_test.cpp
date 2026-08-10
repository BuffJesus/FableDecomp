#include <cstdio>
#include <cstring>

// Behavioural model of DtorIPM_C:
//  - stores vtable ptr 0x129f650 at offset 0
//  - calls member sub-destructor with this
//  - tail-calls base destructor with this

static int g_member_called = 0;
static int g_base_called = 0;
static void* g_member_this = 0;
static void* g_base_this = 0;

struct Obj { unsigned int vtbl; int payload; };

static void member_dtor(Obj* self){ g_member_called++; g_member_this=self; }
static void base_dtor(Obj* self){ g_base_called++; g_base_this=self; }

static void model_dtor(Obj* self){
    self->vtbl = 0x129f650u;
    member_dtor(self);
    base_dtor(self);
}

int main(){
    Obj o; memset(&o,0,sizeof(o)); o.vtbl=0xdeadbeef; o.payload=7;
    model_dtor(&o);
    int ok = 1;
    if(o.vtbl != 0x129f650u) ok=0;
    if(g_member_called != 1) ok=0;
    if(g_base_called != 1) ok=0;
    if(g_member_this != &o) ok=0;
    if(g_base_this != &o) ok=0;
    if(o.payload != 7) ok=0;
    if(ok) printf("OK_00af8c50\n");
    else   printf("FAIL_00af8c50\n");
    return ok?0:1;
}