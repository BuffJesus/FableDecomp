#include <cstdio>

struct Obj { double v; };

static double g_captured = 0.0;
static Obj* g_self = 0;

// definition of the helper the source declares
void __fastcall helper_c0dd60(Obj* self, double d)
{
    g_self = self;
    g_captured = d;
    self->v = d;
}

Obj* __fastcall operator_andand_predicate(Obj* self, double d);

int main()
{
    Obj o; o.v = -1.0;
    Obj* r = operator_andand_predicate(&o, 3.5);
    if (r == &o && g_self == &o && g_captured == 3.5 && o.v == 3.5)
        std::printf("00c0ded0_TEST PASS\n");
    else
        std::printf("FAIL r=%p self=%p cap=%f v=%f\n", (void*)r, (void*)g_self, g_captured, o.v);
    return 0;
}