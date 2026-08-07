#include <cstdio>

struct Member { char pad[4]; };
struct Host { char pad[0x4]; Member m_at; };

static int g_order[4];
static int g_n = 0;

extern "C" void __fastcall FableMemberDtor_009e63f0(Member* p) { g_order[g_n++] = 1; }
extern "C" void __fastcall FableBaseDtor_009e63f0(Host* self) { g_order[g_n++] = 2; }

void __fastcall FableDestVal_009e63f0(Host* self)
{
    FableMemberDtor_009e63f0(&self->m_at);
    FableBaseDtor_009e63f0(self);
}

int main()
{
    Host h;
    FableDestVal_009e63f0(&h);
    if (g_n == 2 && g_order[0] == 1 && g_order[1] == 2) {
        std::printf("DESTVAL_009e63f0_OK\n");
        return 0;
    }
    std::printf("DESTVAL_009e63f0_FAIL\n");
    return 1;
}