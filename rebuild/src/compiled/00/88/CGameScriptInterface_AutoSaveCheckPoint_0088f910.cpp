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

void CGameScriptInterface::AutoSaveCheckPoint() const
{
    const CGameScriptInterface_Overlay* const overlay =
        reinterpret_cast<const CGameScriptInterface_Overlay*>(this);
    overlay->m_pWorld->AutoSaveCheckPoint();
}