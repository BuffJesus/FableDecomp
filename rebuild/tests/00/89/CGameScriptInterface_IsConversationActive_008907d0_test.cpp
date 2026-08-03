#include <cstdio>

class CDisplayManager
{
public:
    bool CopyBackBufferToTexture(long a);
};

class CGameSubSystem
{
public:
    char pad00[0x7C];
    CDisplayManager* m_DisplayManager;
};

class CGameScriptInterface
{
public:
    CGameSubSystem* m_Sub;
    virtual bool IsConversationActive(long a) const;
};

static bool g_called = false;
static CDisplayManager* g_expected = 0;
static long g_arg = 0;

bool CDisplayManager::CopyBackBufferToTexture(long a)
{
    g_called = (this == g_expected);
    g_arg = a;
    return true;
}

bool CGameScriptInterface::IsConversationActive(long a) const
{
    return m_Sub->m_DisplayManager->CopyBackBufferToTexture(a);
}

int main()
{
    CDisplayManager disp;
    CGameSubSystem holder;
    holder.m_DisplayManager = &disp;
    g_expected = &disp;

    CGameScriptInterface gsi;
    gsi.m_Sub = &holder;

    bool r = gsi.IsConversationActive(7);
    if (r && g_called && g_arg == 7)
    {
        std::printf("ISCONV_OK\n");
        return 0;
    }
    std::printf("ISCONV_FAIL\n");
    return 1;
}