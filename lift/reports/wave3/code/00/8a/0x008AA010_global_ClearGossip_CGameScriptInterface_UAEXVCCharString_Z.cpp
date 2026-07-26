#include <cstddef>
#include <cstdint>
#include <functional>
#include <map>

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

    void __thiscall Std_Vector_Erase_Range(T* pFirst, T* pLast);
};

static_assert(offsetof(CArray<CMeshDataBank::CLipSyncEntry>, m_pBegin) == 0x00);
static_assert(offsetof(CArray<CMeshDataBank::CLipSyncEntry>, m_pEnd) == 0x04);
static_assert(sizeof(CArray<CMeshDataBank::CLipSyncEntry>) == 0x08);

using GossipMap = std::map<
    CCharString,
    CArray<CMeshDataBank::CLipSyncEntry>,
    std::less<CCharString>,
    std::allocator<std::pair<const CCharString, CArray<CMeshDataBank::CLipSyncEntry>>>>;

extern GossipMap DAT_013bae44;

class CGameScriptInterface
{
public:
    virtual void __thiscall ClearGossip(CCharString gossipName);
};

void __thiscall CGameScriptInterface::ClearGossip(CCharString gossipName)
{
    CCharString localKey(gossipName);
    CArray<CMeshDataBank::CLipSyncEntry>& entries = DAT_013bae44[localKey];
    entries.Std_Vector_Erase_Range(entries.m_pBegin, entries.m_pEnd);
}