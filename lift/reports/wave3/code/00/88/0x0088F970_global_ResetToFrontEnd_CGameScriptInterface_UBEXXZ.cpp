#include <cstddef>

class CWorld
{
public:
    void __thiscall ResetToFrontEnd();
};

class CGameScriptInterface
{
public:
    void __thiscall ResetToFrontEnd() const;
};

namespace
{
struct CGameScriptInterface_ResetToFrontEnd_SelfOverlay
{
    std::byte m_Pad00[0x04];
    CWorld* m_Field04; // 0x04
};

static_assert(
    offsetof(
        CGameScriptInterface_ResetToFrontEnd_SelfOverlay,
        m_Field04) == 0x04);
}

void __thiscall CGameScriptInterface::ResetToFrontEnd() const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptInterface_ResetToFrontEnd_SelfOverlay*>(this);

    self->m_Field04->ResetToFrontEnd();
}