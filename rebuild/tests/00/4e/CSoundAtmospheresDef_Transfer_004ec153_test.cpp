#include <cstdio>
#include <cstring>

struct CPersistContext
{
    void Transfer(void* p);
};

struct CSoundAtmospheresDef
{
    char pad[0x28];
    int  m_member;
    void Transfer(CPersistContext& ctx);
};

void CSoundAtmospheresDef::Transfer(CPersistContext& ctx)
{
    ctx.Transfer(&this->m_member);
}

// Local definition of the external callee.
static void* g_received = 0;
void CPersistContext::Transfer(void* p)
{
    g_received = p;
}

int main()
{
    CSoundAtmospheresDef def;
    memset(&def, 0, sizeof(def));
    CPersistContext ctx;

    def.Transfer(ctx);

    // Verify the pointer handed to the context is exactly this+0x28.
    unsigned char* base = reinterpret_cast<unsigned char*>(&def);
    void* expected = base + 0x28;

    if (g_received == expected && g_received == &def.m_member)
    {
        printf("ATMOS_TRANSFER_OK\n");
        return 0;
    }
    printf("FAIL got=%p expected=%p\n", g_received, expected);
    return 1;
}