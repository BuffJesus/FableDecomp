#include <cstdio>

typedef unsigned int uint;

struct Sub { int x; };

struct Obj {
    int field0;
    Sub sub;
};

static int g_dtor_called = 0;
static int g_delete_called = 0;
static void* g_dtor_arg = 0;
static void* g_delete_arg = 0;

void __fastcall Sub_dtor(Sub* self) { g_dtor_called++; g_dtor_arg = self; }
void __cdecl op_delete(void* p) { g_delete_called++; g_delete_arg = p; }

void* __fastcall vdd(Obj* self, int /*edx*/, uint flags)
{
    Sub_dtor(&self->sub);
    if (flags & 1)
        op_delete(self);
    return self;
}

int main()
{
    Obj a;
    // flags without bit0: destructor runs, no delete, returns self
    g_dtor_called = g_delete_called = 0;
    void* r0 = vdd(&a, 0, 0);
    // flags with bit0: destructor runs, delete runs
    void* r1 = vdd(&a, 0, 1);

    bool ok = true;
    if (r0 != &a || r1 != &a) ok = false;
    if (g_dtor_called != 2) ok = false;          // dtor both times
    if (g_delete_called != 1) ok = false;         // delete only when bit0
    if (g_dtor_arg != &a.sub) ok = false;         // dtor on subobject +4
    if (g_delete_arg != &a) ok = false;           // delete on whole obj

    if (ok) printf("VDD_PARITY_OK\n");
    else    printf("FAIL d=%d del=%d\n", g_dtor_called, g_delete_called);
    return 0;
}