#include <cstdio>

struct Sub { int x; };
struct Obj { char pad[0xA8]; Sub sub; int flag; };

static int g_helper_called = 0;
static int g_tail_called = 0;
static Sub* g_helper_arg = 0;
static Obj* g_tail_arg = 0;

void helper(Sub* s) { g_helper_called = 1; g_helper_arg = s; }
void tail(Obj* o) { g_tail_called = 1; g_tail_arg = o; }

void model(Obj* self) {
    helper(&self->sub);
    tail(self);
}

int main() {
    Obj o;
    o.flag = 42;
    model(&o);
    int ok = 1;
    if (!g_helper_called) ok = 0;
    if (!g_tail_called) ok = 0;
    if (g_helper_arg != &o.sub) ok = 0;
    if (g_tail_arg != &o) ok = 0;
    if ((char*)g_helper_arg - (char*)&o != 0xA8) ok = 0;
    if (ok) printf("OK_00947bc0\n");
    else printf("FAIL\n");
    return ok ? 0 : 1;
}