#include <cstddef>
#include <cstdint>

struct CGameScriptInterfaceOwnerOverlay
{
    std::byte m_Pad00[0x4];
    void* m_pOwner; // 0x4
};
static_assert(offsetof(CGameScriptInterfaceOwnerOverlay, m_pOwner) == 0x4);

struct CGameScriptInterfaceGuildMasterMessagesOwnerOverlay
{
    std::byte m_Pad00[0xE0];
    std::uint8_t m_bGuildMasterMessages; // 0xE0
};
static_assert(offsetof(CGameScriptInterfaceGuildMasterMessagesOwnerOverlay, m_bGuildMasterMessages) == 0xE0);

class CGameScriptInterface
{
public:
    void __thiscall SetGuildMasterMessages(std::uint8_t enabled) const;
};

void __thiscall CGameScriptInterface::SetGuildMasterMessages(std::uint8_t enabled) const
{
    auto* const ownerHolder = reinterpret_cast<const CGameScriptInterfaceOwnerOverlay*>(this);
    auto* const owner = reinterpret_cast<CGameScriptInterfaceGuildMasterMessagesOwnerOverlay*>(ownerHolder->m_pOwner);
    owner->m_bGuildMasterMessages = enabled;
}