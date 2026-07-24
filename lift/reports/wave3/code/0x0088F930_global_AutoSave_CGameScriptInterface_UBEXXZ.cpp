#include <cstddef>

class CWorld
{
public:
    void __thiscall AutoSave(bool bAutoSaveCheckpoint);
};

class CGameScriptInterface
{
public:
    void __thiscall AutoSave() const;
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

void CGameScriptInterface::AutoSave() const
{
    const auto* const overlay = reinterpret_cast<const CGameScriptInterface_Overlay*>(this);
    overlay->m_pWorld->AutoSave(false);
}