#include <stdio.h>

static int g_init_called = 0;
static void* g_init_this = 0;
static int g_fwd_called = 0;
static void* g_fwd_this = 0;

struct Obj { char pad[0xa8]; int member; };

static void init_member(void* self){ g_init_called++; g_init_this = self; }
static void forward(void* self){ g_fwd_called++; g_fwd_this = self; }

// behavioural model of DestVal_A8_A: init subobject at +0xa8 then forward with this
static void model(void* self)
{
    char* p = (char*)self;
    init_member(p + 0xa8);
    forward(self);
}

int main(void)
{
    Obj o;
    model(&o);
    if (g_init_called != 1) { printf("FAIL init count\n"); return 1; }
    if (g_init_this != (char*)&o + 0xa8) { printf("FAIL init this\n"); return 1; }
    if (g_fwd_called != 1) { printf("FAIL fwd count\n"); return 1; }
    if (g_fwd_this != (void*)&o) { printf("FAIL fwd this\n"); return 1; }
    printf("OK_0085edf0\n");
    return 0;
}