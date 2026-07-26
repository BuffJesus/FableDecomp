#include <cstddef>

struct CCharString
{
    std::byte m_Data[0x04];
};

static_assert(sizeof(CCharString) == 0x04);

class CGameScriptInterface
{
public:
    virtual void __thiscall RemoveGossip(CCharString gossipName);
};

namespace
{
struct CGameScriptInterface_RemoveGossip_GlobalOverlay
{
    std::byte m_Opaque;
};

static_assert(sizeof(CGameScriptInterface_RemoveGossip_GlobalOverlay) == 0x01);

using CCharString_CopyConstructFn =
    void(__thiscall*)(CCharString* pThis, const CCharString* pOther);

using CCharString_DestroyFn =
    void(__thiscall*)(CCharString* pThis);

using CGameScriptInterface_RemoveGossip_HelperFn =
    void(__thiscall*)(CGameScriptInterface_RemoveGossip_GlobalOverlay* pThis, const CCharString* pKey);

inline auto* const CCharString__CCharString =
    reinterpret_cast<CCharString_CopyConstructFn>(0x0099EC30);

inline auto* const CCharString__dtor =
    reinterpret_cast<CCharString_DestroyFn>(0x0099EAE0);

inline auto* const CGameScriptInterface_RemoveGossip_Helper =
    reinterpret_cast<CGameScriptInterface_RemoveGossip_HelperFn>(0x008AE400);

inline auto* const DAT_013BAE44 =
    reinterpret_cast<CGameScriptInterface_RemoveGossip_GlobalOverlay*>(0x013BAE44);
}

void __thiscall CGameScriptInterface::RemoveGossip(CCharString gossipName)
{
    alignas(CCharString) std::byte gossipNameCopyStorage[sizeof(CCharString)];
    auto* const pGossipNameCopy = reinterpret_cast<CCharString*>(gossipNameCopyStorage);

    CCharString__CCharString(pGossipNameCopy, &gossipName);
    CGameScriptInterface_RemoveGossip_Helper(DAT_013BAE44, pGossipNameCopy);
    CCharString__dtor(pGossipNameCopy);
    CCharString__dtor(&gossipName);
}