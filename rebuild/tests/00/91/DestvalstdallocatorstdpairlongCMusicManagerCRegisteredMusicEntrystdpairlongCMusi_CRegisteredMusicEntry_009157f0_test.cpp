#include <cstdio>

struct Sub { int a; };
struct Obj { char pad[0xA8]; Sub sub; };

static int g_sub = 0;
static int g_base = 0;
static void* g_subarg = 0;
static void* g_basearg = 0;

void SubDtor(Sub* p) { g_sub++; g_subarg = (void*)p; }
void BaseDtor(void* self) { g_base++; g_basearg = self; }

// model of the target
void DestVal_A8_B(Obj* self)
{
    SubDtor(&self->sub);
    BaseDtor(self);
}

int main()
{
    Obj o;
    DestVal_A8_B(&o);
    int ok = 1;
    if (g_sub != 1) ok = 0;
    if (g_base != 1) ok = 0;
    // sub-object arg must be this + 0xA8
    if (g_subarg != (void*)((char*)&o + 0xA8)) ok = 0;
    // base arg must be this
    if (g_basearg != (void*)&o) ok = 0;
    if (ok) printf("OK_009157f0\n");
    else printf("FAIL_009157f0\n");
    return ok ? 0 : 1;
}