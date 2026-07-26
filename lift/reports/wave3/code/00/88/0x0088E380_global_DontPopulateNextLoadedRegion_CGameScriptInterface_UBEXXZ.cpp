#include <cstddef>
#include <cstdint>

struct CGameScriptInterfaceOwnerOverlay
{
    std::byte m_Pad00[0x4];
    void* m_pOwner; // 0x4
};
static_assert(offsetof(CGameScriptInterfaceOwnerOverlay, m_pOwner) == 0x4);

struct CGameScriptInterfaceDontPopulateOwnerOverlay
{
    std::byte m_Pad00[0xD8];
    std::uint8_t m_bDontPopulateNextLoadedRegion; // 0xD8
};
static_assert(offsetof(CGameScriptInterfaceDontPopulateOwnerOverlay, m_bDontPopulateNextLoadedRegion) == 0xD8);

class CGameScriptInterface
{
public:
    void __thiscall DontPopulateNextLoadedRegion() const;
};

void __thiscall CGameScriptInterface::DontPopulateNextLoadedRegion() const
{
    auto* const ownerHolder = reinterpret_cast<const CGameScriptInterfaceOwnerOverlay*>(this);
    auto* const owner = reinterpret_cast<CGameScriptInterfaceDontPopulateOwnerOverlay*>(ownerHolder->m_pOwner);
    owner->m_bDontPopulateNextLoadedRegion = 1;
}