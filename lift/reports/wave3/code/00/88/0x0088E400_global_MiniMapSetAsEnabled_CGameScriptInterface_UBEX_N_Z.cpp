#include <cstddef>
#include <cstdint>

class CWorld
{
public:
    static void __thiscall SetMiniMapEnabled(CWorld* pWorld, bool enabled);
};

class CGameScriptInterface
{
public:
    void __thiscall MiniMapSetAsEnabled(bool enabled) const;
};

namespace
{
struct CGameScriptInterfaceWorldOverlay
{
    std::byte pad_00[0x04];
    CWorld* pWorld;
};

static_assert(offsetof(CGameScriptInterfaceWorldOverlay, pWorld) == 0x04);
}

void __thiscall CGameScriptInterface::MiniMapSetAsEnabled(bool enabled) const
{
    const auto* const pOverlay =
        reinterpret_cast<const CGameScriptInterfaceWorldOverlay*>(this);
    CWorld::SetMiniMapEnabled(pOverlay->pWorld, enabled);
}