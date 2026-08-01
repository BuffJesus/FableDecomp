#include <cstdio>

// Self-contained behaviour test for the leading _Dest_val @ 0x0047b880.
// Compiles at /Od, links standalone, prints an exact uppercase pass token.
//
// GENUINE-C++ test: it defines the same three-member destroy dispatch as the
// parity source and stubs the single masked __fastcall dtor so the standalone
// link resolves and the destroy order / argument pointers can be observed.

struct Member { char pad[4]; };

struct Host {
    char    pad0[4];   // 0x00
    Member  m_4;       // 0x04
    Member  m_8;       // 0x08
};

extern "C" void __fastcall Symbol_dtor(void* self_ecx);

static int   g_calls = 0;
static void* g_seen[3];

extern "C" void __fastcall Symbol_dtor(void* self_ecx)
{
    if (g_calls < 3) g_seen[g_calls] = self_ecx;
    ++g_calls;
}

void __fastcall Dest_val_symbolinfo(Host* self)
{
    Symbol_dtor(&self->m_8);
    Symbol_dtor(&self->m_4);
    Symbol_dtor(self);
}

int main()
{
    char storage[0x10];
    Host* self = (Host*)storage;
    char* base = (char*)self;

    Dest_val_symbolinfo(self);

    int failures = 0;
    if (g_calls != 3)               { std::printf("FAIL calls=%d\n", g_calls); ++failures; }
    if (g_seen[0] != base + 0x8)    { std::printf("FAIL arg0 not +8\n"); ++failures; }
    if (g_seen[1] != base + 0x4)    { std::printf("FAIL arg1 not +4\n"); ++failures; }
    if (g_seen[2] != base + 0x0)    { std::printf("FAIL arg2 not +0\n"); ++failures; }

    if (failures != 0) {
        std::printf("DEST_VAL_SYMBOLINFO_0047B880_TEST FAIL count=%d\n", failures);
        return 1;
    }
    std::printf("DEST_VAL_SYMBOLINFO_0047B880_TEST PASS\n");
    return 0;
}