#include <cstdio>

struct Member { char pad[4]; };
struct Host { char pad[0x14]; Member m_14; };

static int g_order[4];
static int g_n = 0;

extern "C" void __fastcall FableMemberDtor_008fdc43(Member* p) { g_order[g_n++] = 1; }
extern "C" void __fastcall FableBaseDtor_008fdc43(Host* self) { g_order[g_n++] = 2; }

void __fastcall FableDestVal_008fdc43(Host* self)
{
    FableMemberDtor_008fdc43(&self->m_14);
    FableBaseDtor_008fdc43(self);
}

int main()
{
    Host h;
    FableDestVal_008fdc43(&h);
    if (g_n == 2 && g_order[0] == 1 && g_order[1] == 2) {
        std::printf("DESTVAL_008fdc43_OK\n");
        return 0;
    }
    std::printf("DESTVAL_008fdc43_FAIL\n");
    return 1;
}