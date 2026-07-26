#include <cstddef>
#include <cstdint>

class CCharString
{
public:
    CCharString(const CCharString& other);
    ~CCharString();
};

static_assert(sizeof(CCharString) == 0x04);

class CMeshDataBank
{
public:
    struct CLipSyncEntry;
};

template <typename T>
class CArray
{
public:
    T* m_pBegin; // 0x00
    T* m_pEnd;   // 0x04

    void __thiscall EraseRange(T* pFirst, T* pLast);
};

static_assert(offsetof(CArray<CMeshDataBank::CLipSyncEntry>, m_pBegin) == 0x00);
static_assert(offsetof(CArray<CMeshDataBank::CLipSyncEntry>, m_pEnd) == 0x04);
static_assert(sizeof(CArray<CMeshDataBank::CLipSyncEntry>) == 0x08);

class GossipFactionLipSyncMap
{
public:
    CArray<CMeshDataBank::CLipSyncEntry>& __thiscall operator[](
        const CCharString& key);
};

extern GossipFactionLipSyncMap DAT_013bae5c;

class CGameScriptInterface
{
public:
    virtual void __thiscall ClearGossipFactions(CCharString gossipFaction);
};

void __thiscall CGameScriptInterface::ClearGossipFactions(CCharString gossipFaction)
{
    CCharString gossipFactionCopy(gossipFaction);
    CArray<CMeshDataBank::CLipSyncEntry>& entries =
        DAT_013bae5c[gossipFactionCopy];
    entries.EraseRange(entries.m_pBegin, entries.m_pEnd);
}