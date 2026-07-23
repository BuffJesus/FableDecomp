#include <cstdio>

struct MemberA { char pad[4]; };
struct MemberB { char pad[4]; };
struct Base { char pad[4]; };

static int g_order[8];
static int g_n = 0;

extern "C" void __fastcall dtorA(MemberA* p) { g_order[g_n++] = 1; }
extern "C" void __fastcall dtorB(MemberB* p) { g_order[g_n++] = 2; }
extern "C" void __fastcall dtorBase(Base* p) { g_order[g_n++] = 3; }

struct Host {
    char pad0[0xc];
    MemberB m_c;
    MemberB m_10;
    MemberA m_14;
};

void __fastcall Dest_val(Host* self)
{
    dtorA(&self->m_14);
    dtorB(&self->m_10);
    dtorB(&self->m_c);
    dtorBase((Base*)self);
}

int main()
{
    Host h;
    Dest_val(&h);
    if (g_n == 4 && g_order[0] == 1 && g_order[1] == 2 &&
        g_order[2] == 2 && g_order[3] == 3) {
        std::printf("_Dest_val_004047a0_TEST PASS\n");
        return 0;
    }
    std::printf("_Dest_val_004047a0_TEST FAIL\n");
    return 1;
}