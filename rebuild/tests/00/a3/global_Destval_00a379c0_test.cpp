#include <cstdio>

struct Member { char pad[4]; };
struct Host { char pad[0x10]; Member m_at; };

static int g_order[4];
static int g_n = 0;

extern "C" void __fastcall FableMemberDtor_00a379c0(Member* p) { g_order[g_n++] = 1; }
extern "C" void __fastcall FableBaseDtor_00a379c0(Host* self) { g_order[g_n++] = 2; }

void __fastcall FableDestVal_00a379c0(Host* self)
{
    FableMemberDtor_00a379c0(&self->m_at);
    FableBaseDtor_00a379c0(self);
}

int main()
{
    Host h;
    FableDestVal_00a379c0(&h);
    if (g_n == 2 && g_order[0] == 1 && g_order[1] == 2) {
        std::printf("DESTVAL_00a379c0_OK\n");
        return 0;
    }
    std::printf("DESTVAL_00a379c0_FAIL\n");
    return 1;
}