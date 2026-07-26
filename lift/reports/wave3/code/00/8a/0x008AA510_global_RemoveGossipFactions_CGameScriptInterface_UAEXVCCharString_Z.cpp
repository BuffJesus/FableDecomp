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
    virtual void __thiscall RemoveGossipFactions(CCharString gossipFaction);
};

namespace
{
struct GossipFactionLipSyncMapOverlay
{
    std::byte m_Opaque;
};

static_assert(sizeof(GossipFactionLipSyncMapOverlay) == 0x01);

using CCharString_CopyConstructFn =
    void(__thiscall*)(CCharString* pThis, const CCharString* pOther);

using CCharString_DestroyFn =
    void(__thiscall*)(CCharString* pThis);

using GossipFactionLipSyncMap_RemoveFn =
    void(__thiscall*)(GossipFactionLipSyncMapOverlay* pThis, const CCharString* pKey);

inline auto* const CCharString__CCharString =
    reinterpret_cast<CCharString_CopyConstructFn>(0x0099EC30);

inline auto* const CCharString__dtor =
    reinterpret_cast<CCharString_DestroyFn>(0x0099EAE0);

inline auto* const GossipFactionLipSyncMap__Remove =
    reinterpret_cast<GossipFactionLipSyncMap_RemoveFn>(0x008AE420);

inline auto* const DAT_013BAE5C =
    reinterpret_cast<GossipFactionLipSyncMapOverlay*>(0x013BAE5C);
}

void __thiscall CGameScriptInterface::RemoveGossipFactions(CCharString gossipFaction)
{
    alignas(CCharString) std::byte gossipFactionCopyStorage[sizeof(CCharString)];
    auto* const pGossipFactionCopy =
        reinterpret_cast<CCharString*>(gossipFactionCopyStorage);

    CCharString__CCharString(pGossipFactionCopy, &gossipFaction);
    GossipFactionLipSyncMap__Remove(DAT_013BAE5C, pGossipFactionCopy);
    CCharString__dtor(pGossipFactionCopy);
    CCharString__dtor(&gossipFaction);
}