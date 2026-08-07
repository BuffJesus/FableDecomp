#include <cstdio>

struct Member { char pad[4]; };
struct Host { char pad[0x14]; Member m_at; };

static int g_order[4];
static int g_n = 0;

extern "C" void __fastcall FableMemberDtor_008fe5dd(Member* p) { g_order[g_n++] = 1; }
extern "C" void __fastcall FableBaseDtor_008fe5dd(Host* self) { g_order[g_n++] = 2; }

void __fastcall FableDestVal_008fe5dd(Host* self)
{
    FableMemberDtor_008fe5dd(&self->m_at);
    FableBaseDtor_008fe5dd(self);
}

int main()
{
    Host h;
    FableDestVal_008fe5dd(&h);
    if (g_n == 2 && g_order[0] == 1 && g_order[1] == 2) {
        std::printf("DESTVAL_008fe5dd_OK\n");
        return 0;
    }
    std::printf("DESTVAL_008fe5dd_FAIL\n");
    return 1;
}