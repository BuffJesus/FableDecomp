#include <cstdio>

struct CMacro { char pad[4]; };

static void* g_order[8];
static int g_n = 0;

extern "C" void __fastcall CMacro_dtor(CMacro* self) { g_order[g_n++] = (void*)self; }

struct Host {
    CMacro m_0;   // +0x00
    CMacro m_4;   // +0x04
    CMacro m_8;   // +0x08
    CMacro m_c;   // +0x0c
};

void __fastcall Dest_val(Host* self)
{
    CMacro_dtor(&self->m_c);
    CMacro_dtor(&self->m_8);
    CMacro_dtor(&self->m_4);
    CMacro_dtor(&self->m_0);
}

int main()
{
    Host h;
    char* base = (char*)&h;
    Dest_val(&h);
    int ok = (g_n == 4)
        && (g_order[0] == (void*)(base + 0xc))
        && (g_order[1] == (void*)(base + 0x8))
        && (g_order[2] == (void*)(base + 0x4))
        && (g_order[3] == (void*)(base + 0x0));
    if (ok) {
        std::printf("DEST_VAL_CMACRO_004404B0_TEST PASS\n");
        return 0;
    }
    std::printf("DEST_VAL_CMACRO_004404B0_TEST FAIL n=%d\n", g_n);
    return 1;
}