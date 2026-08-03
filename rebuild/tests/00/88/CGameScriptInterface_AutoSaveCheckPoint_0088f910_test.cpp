#include <cstddef>

class CWorld
{
public:
    void AutoSaveCheckPoint();
};

class CGameScriptInterface
{
public:
    virtual void AutoSaveCheckPoint() const;
};

struct CGameScriptInterface_Overlay
{
    unsigned char m_Pad00[0x4];
    CWorld* m_pWorld; // 0x4
};

static bool g_called = false;
static CWorld* g_expected = 0;

void CWorld::AutoSaveCheckPoint()
{
    g_called = (reinterpret_cast<CWorld*>(this) == g_expected);
}

void CGameScriptInterface::AutoSaveCheckPoint() const
{
    const CGameScriptInterface_Overlay* const overlay =
        reinterpret_cast<const CGameScriptInterface_Overlay*>(this);
    overlay->m_pWorld->AutoSaveCheckPoint();
}

#include <cstdio>

int main()
{
    CWorld world;
    g_expected = &world;

    struct Layout
    {
        void* vtbl;
        CWorld* pWorld;
    } obj;
    obj.vtbl = 0;
    obj.pWorld = &world;

    CGameScriptInterface* gsi = reinterpret_cast<CGameScriptInterface*>(&obj);
    gsi->CGameScriptInterface::AutoSaveCheckPoint();

    if (g_called)
    {
        printf("AUTOSAVE_FWD_OK\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}