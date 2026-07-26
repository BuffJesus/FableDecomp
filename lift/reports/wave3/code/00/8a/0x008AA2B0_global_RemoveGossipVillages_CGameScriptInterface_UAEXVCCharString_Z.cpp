#include <cstddef>

class CCharString
{
public:
    CCharString(const CCharString& other);
    ~CCharString();
};

static_assert(sizeof(CCharString) == 0x04);

class CGameScriptInterface
{
public:
    virtual void __thiscall RemoveGossipVillages(CCharString gossipVillage);
};

namespace
{
struct CGameScriptInterface_RemoveGossipVillages_GlobalOverlay
{
    std::byte m_Opaque;
};

static_assert(sizeof(CGameScriptInterface_RemoveGossipVillages_GlobalOverlay) == 0x01);

using CCharString_CopyConstructFn =
    void(__thiscall*)(CCharString* pThis, const CCharString* pOther);

using CCharString_DestroyFn =
    void(__thiscall*)(CCharString* pThis);

using CGameScriptInterface_RemoveGossipVillages_HelperFn =
    void(__thiscall*)(CGameScriptInterface_RemoveGossipVillages_GlobalOverlay* pThis, const CCharString* pKey);

inline auto* const CCharString__CCharString =
    reinterpret_cast<CCharString_CopyConstructFn>(0x0099EC30);

inline auto* const CCharString__dtor =
    reinterpret_cast<CCharString_DestroyFn>(0x0099EAE0);

inline auto* const CGameScriptInterface_RemoveGossipVillages_Helper =
    reinterpret_cast<CGameScriptInterface_RemoveGossipVillages_HelperFn>(0x008AE420);

inline auto* const DAT_013BAE50 =
    reinterpret_cast<CGameScriptInterface_RemoveGossipVillages_GlobalOverlay*>(0x013BAE50);
}

void __thiscall CGameScriptInterface::RemoveGossipVillages(CCharString gossipVillage)
{
    alignas(CCharString) std::byte gossipVillageCopyStorage[sizeof(CCharString)];
    auto* const pGossipVillageCopy =
        reinterpret_cast<CCharString*>(gossipVillageCopyStorage);

    CCharString__CCharString(pGossipVillageCopy, &gossipVillage);
    CGameScriptInterface_RemoveGossipVillages_Helper(DAT_013BAE50, pGossipVillageCopy);
    CCharString__dtor(pGossipVillageCopy);
    CCharString__dtor(&gossipVillage);
}