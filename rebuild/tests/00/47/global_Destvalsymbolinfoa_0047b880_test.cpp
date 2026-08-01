#include <cstdio>

static int  g_sub_calls = 0;
static int  g_base_calls = 0;
static char* g_last_sub[2];
static char* g_last_base = 0;

// The masked callees of routine 1 (the only path a __fastcall(self) exercises;
// routine 2 is unreachable dead code after the tail-jmp).
extern "C" void __fastcall _Dest_val_sub_dtor(void* member_ecx)
{
    if (g_sub_calls < 2) g_last_sub[g_sub_calls] = (char*)member_ecx;
    g_sub_calls++;
}
extern "C" void __fastcall _Dest_val_base_dtor(void* self_ecx)
{
    g_base_calls++;
    g_last_base = (char*)self_ecx;
}
// Routine 2's callees are never reached at runtime but must resolve at link time.
extern "C" void* __cdecl _Dest_val_operator_new(unsigned int) { return 0; }
extern "C" void __fastcall _Dest_val_construct(void*, unsigned int, void*) {}

extern "C" void __fastcall _Dest_val_symbolinfo_a(void* self);

int main()
{
    char buf[0x50];
    char* base = buf;
    _Dest_val_symbolinfo_a(base);
    if (g_sub_calls != 2) { std::printf("FAIL sub_calls=%d\n", g_sub_calls); return 1; }
    if (g_base_calls != 1) { std::printf("FAIL base_calls=%d\n", g_base_calls); return 1; }
    if (g_last_sub[0] != base + 0x8) { std::printf("FAIL first sub offset\n"); return 1; }
    if (g_last_sub[1] != base + 0x4) { std::printf("FAIL second sub offset\n"); return 1; }
    if (g_last_base != base) { std::printf("FAIL base ptr\n"); return 1; }
    std::printf("_DEST_VAL_0047B880_TEST PASS\n");
    return 0;
}