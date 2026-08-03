#include <cstdio>

static int g_called = 0;

class CWorld
{
public:
    void ResetToFrontEnd() { g_called = 1; }
};

class CGameScriptInterface
{
public:
    void ResetToFrontEnd() const;
    void* m_Pad00;
    CWorld* m_World;
};

void CGameScriptInterface::ResetToFrontEnd() const
{
    m_World->ResetToFrontEnd();
}

int main()
{
    CWorld world;
    CGameScriptInterface gsi;
    gsi.m_Pad00 = 0;
    gsi.m_World = &world;
    gsi.ResetToFrontEnd();
    if (g_called == 1) { std::printf("RESET_FRONTEND_OK\n"); return 0; }
    std::printf("FAIL\n"); return 1;
}