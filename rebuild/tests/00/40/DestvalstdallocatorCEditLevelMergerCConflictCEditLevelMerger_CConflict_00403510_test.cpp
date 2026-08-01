#include <cstdio>

struct MemberA { char pad[4]; };
struct MemberB { char pad[4]; };

static int   g_seq   = 0;
static void* g_ptr[8];
static int   g_kind[8];   // 1 = dtorA, 2 = dtorB
static int   g_n     = 0;

extern "C" void __fastcall dtorA(MemberA* p) { g_ptr[g_n] = p; g_kind[g_n] = 1; ++g_n; ++g_seq; }
extern "C" void __fastcall dtorB(MemberB* p) { g_ptr[g_n] = p; g_kind[g_n] = 2; ++g_n; ++g_seq; }

struct Host {
    char     pad0[0x0c];
    MemberA  m_0c;
    char     pad1[0x2c - 0x10];
    MemberB  m_2c;
    MemberB  m_30;
    char     pad2[0x94 - 0x34];
    MemberA  m_94;
    char     pad3[0xb8 - 0x98];
    MemberA  m_b8;
    MemberA  m_bc;
};

void __fastcall Dest_val(Host* self)
{
    dtorA(&self->m_bc);
    dtorA(&self->m_b8);
    dtorA(&self->m_94);
    dtorB(&self->m_30);
    dtorB(&self->m_2c);
    dtorA(&self->m_0c);
}

int main()
{
    unsigned char buf[0x100];
    Host* self = reinterpret_cast<Host*>(buf);
    unsigned char* base = buf;

    Dest_val(self);

    if (g_n != 6) { std::printf("FAIL n=%d\n", g_n); return 1; }

    // Exact offsets, kinds, and order.
    struct Exp { int off; int kind; } exp[6] = {
        { 0xbc, 1 }, { 0xb8, 1 }, { 0x94, 1 },
        { 0x30, 2 }, { 0x2c, 2 }, { 0x0c, 1 }
    };
    for (int i = 0; i < 6; ++i) {
        if (g_ptr[i] != (void*)(base + exp[i].off)) { std::printf("FAIL ptr%d\n", i); return 2; }
        if (g_kind[i] != exp[i].kind)               { std::printf("FAIL kind%d\n", i); return 3; }
    }

    std::printf("FABLETLC_DESTVAL_CCONFLICT_00403510 PASS\n");
    return 0;
}