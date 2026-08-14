// Behaviour model for the vector-deleting-destructor.
#include <cstdio>

typedef unsigned int uint;

static int g_dtor_calls = 0;
static void* g_dtor_arg = 0;
static int g_delete_calls = 0;
static void* g_delete_arg = 0;

struct Patch { char pad[64]; };

static void __fastcall sub_dtor(void* subobj) {
    g_dtor_calls++;
    g_dtor_arg = subobj;
}
static void __cdecl my_delete(void* p) {
    g_delete_calls++;
    g_delete_arg = p;
}

static void* __fastcall vdd(Patch* self, int /*edx*/, uint flags) {
    sub_dtor((char*)self + 4);
    if (flags & 1)
        my_delete(self);
    return self;
}

int main() {
    Patch p;
    // flags bit0 = 0 -> no delete
    g_dtor_calls = g_delete_calls = 0;
    void* r = vdd(&p, 0, 0);
    bool ok = (r == &p) && (g_dtor_calls == 1)
              && (g_dtor_arg == (char*)&p + 4) && (g_delete_calls == 0);

    // flags bit0 = 1 -> delete
    g_dtor_calls = g_delete_calls = 0;
    r = vdd(&p, 0, 1);
    ok = ok && (r == &p) && (g_dtor_calls == 1)
       && (g_dtor_arg == (char*)&p + 4)
       && (g_delete_calls == 1) && (g_delete_arg == &p);

    // even flags value with bit0 set (3) still deletes
    g_dtor_calls = g_delete_calls = 0;
    r = vdd(&p, 0, 3);
    ok = ok && (g_delete_calls == 1);

    // flags 2 -> bit0 clear -> no delete
    g_dtor_calls = g_delete_calls = 0;
    r = vdd(&p, 0, 2);
    ok = ok && (g_delete_calls == 0) && (g_dtor_calls == 1);

    if (ok) printf("VDD_OK\n");
    else printf("VDD_FAIL\n");
    return 0;
}