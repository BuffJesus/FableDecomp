#include <cstdio>

struct Member { char pad[4]; };
struct Host { char pad[0x14]; Member m_14; };

static int g_order[4];
static int g_n = 0;

extern "C" void __fastcall FableMemberDtor_008fddaa(Member* p) { g_order[g_n++] = 1; }
extern "C" void __fastcall FableBaseDtor_008fddaa(Host* self) { g_order[g_n++] = 2; }

void __fastcall FableDestVal_008fddaa(Host* self)
{
    FableMemberDtor_008fddaa(&self->m_14);
    FableBaseDtor_008fddaa(self);
}

int main()
{
    Host h;
    FableDestVal_008fddaa(&h);
    if (g_n == 2 && g_order[0] == 1 && g_order[1] == 2) {
        std::printf("DESTVAL_008fddaa_OK\n");
        return 0;
    }
    std::printf("DESTVAL_008fddaa_FAIL\n");
    return 1;
}