#include <cstddef>

class CWorld
{
public:
    void __thiscall AutoSaveCheckPoint();
};

class CGameScriptInterface
{
public:
    void __thiscall AutoSaveCheckPoint() const;
};

namespace
{
struct CGameScriptInterface_Overlay
{
    std::byte m_Pad00[0x4];
    CWorld* m_pWorld; // 0x4
};
static_assert(offsetof(CGameScriptInterface_Overlay, m_pWorld) == 0x4);
}

void CGameScriptInterface::AutoSaveCheckPoint() const
{
    const auto* const overlay = reinterpret_cast<const CGameScriptInterface_Overlay*>(this);
    overlay->m_pWorld->AutoSaveCheckPoint();
}