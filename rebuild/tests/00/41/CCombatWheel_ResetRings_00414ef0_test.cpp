#include <cstdio>
struct Ring { int dummy; };
static int g_dtor_calls = 0;
static int g_free_calls = 0;
void __fastcall Ring_dtor(Ring* r) { (void)r; g_dtor_calls++; }
void __cdecl op_free(void* p) { (void)p; g_free_calls++; }
struct RingVec { Ring* first; Ring* last; };

void __fastcall CCombatWheel_ResetRings(RingVec* self)
{
    Ring* p = self->first;
    Ring* e = self->last;
    while (p != e) {
        Ring_dtor(p);
        p++;
    }
    if (self->first)
        op_free(self->first);
}

int main() {
    static Ring buf[3];
    RingVec v;
    v.first = buf;
    v.last = buf + 3;
    CCombatWheel_ResetRings(&v);
    if (g_dtor_calls == 3 && g_free_calls == 1) {
        std::printf("CCombatWheel_00414ef0_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL dtor=%d free=%d\n", g_dtor_calls, g_free_calls);
    return 1;
}