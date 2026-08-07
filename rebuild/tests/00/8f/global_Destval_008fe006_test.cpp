#include <cstdio>

struct Member { char pad[4]; };
struct Host { char pad[0x14]; Member m_14; };

static int g_order[4];
static int g_n = 0;

extern "C" void __fastcall FableMemberDtor_008fe006(Member* p) { g_order[g_n++] = 1; }
extern "C" void __fastcall FableBaseDtor_008fe006(Host* self) { g_order[g_n++] = 2; }

void __fastcall FableDestVal_008fe006(Host* self)
{
    FableMemberDtor_008fe006(&self->m_14);
    FableBaseDtor_008fe006(self);
}

int main()
{
    Host h;
    FableDestVal_008fe006(&h);
    if (g_n == 2 && g_order[0] == 1 && g_order[1] == 2) {
        std::printf("DESTVAL_008fe006_OK\n");
        return 0;
    }
    std::printf("DESTVAL_008fe006_FAIL\n");
    return 1;
}