#include <cstdio>

// Behaviour fixture for _Dest_val forwarder @ 0x008fd8d7. Confirms member
// destructor runs before the base/next destructor.
struct Member { char pad[4]; };
struct Host { char pad[0x14]; Member m_14; };

static int g_order[4];
static int g_n = 0;

extern "C" void __fastcall FableMemberDtor_008fd8d7(Member* p) { g_order[g_n++] = 1; }
extern "C" void __fastcall FableBaseDtor_008fd8d7(Host* self) { g_order[g_n++] = 2; }

void __fastcall FableDestVal_008fd8d7(Host* self)
{
    FableMemberDtor_008fd8d7(&self->m_14);
    FableBaseDtor_008fd8d7(self);
}

int main()
{
    Host h;
    FableDestVal_008fd8d7(&h);
    if (g_n == 2 && g_order[0] == 1 && g_order[1] == 2) {
        std::printf("DESTVAL_008fd8d7_OK\n");
        return 0;
    }
    std::printf("DESTVAL_008fd8d7_FAIL\n");
    return 1;
}