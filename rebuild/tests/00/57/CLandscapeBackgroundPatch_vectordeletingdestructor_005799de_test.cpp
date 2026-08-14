#include <cstdio>
struct Inner { int x; };
static int g_dtor_calls = 0; static int g_delete_calls = 0;
void __fastcall Inner_dtor(Inner* self) { g_dtor_calls++; (void)self; }
extern "C" void __cdecl sub_operator_delete(void* p) { g_delete_calls++; (void)p; }
struct CLandscapeBackgroundPatch { void* pad0; Inner inner; };
void* __fastcall vdd(CLandscapeBackgroundPatch* self, unsigned int, unsigned int flags)
{ Inner_dtor((Inner*)((char*)self + 4)); if (flags & 1) sub_operator_delete(self); return self; }
int main() {
    CLandscapeBackgroundPatch obj;
    g_dtor_calls = g_delete_calls = 0;
    void* r = vdd(&obj, 0, 0);
    if (r != &obj || g_dtor_calls != 1 || g_delete_calls != 0) { printf("BAD\n"); return 1; }
    g_dtor_calls = g_delete_calls = 0;
    r = vdd(&obj, 0, 1);
    if (r != &obj || g_dtor_calls != 1 || g_delete_calls != 1) { printf("BAD\n"); return 1; }
    printf("VDD_OK\n"); return 0;
}