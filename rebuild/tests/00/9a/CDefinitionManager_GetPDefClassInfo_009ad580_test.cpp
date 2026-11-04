// Behaviour of CDefinitionManager::GetPDefClassInfo(const CCharString&) 0x009AD580:
// looks the class up by the CRC of its name (seed 0; an empty string hashes
// the shared empty buffer with length 0) in DefClassInfo through LowerBound;
// returns the info of the entry with exactly that key, or null when the
// lower bound is the end or a larger key.
#include "fable_definition_manager.h"
#include <stdio.h>
#include <string.h>

typedef CDefinitionManager::CDefClassInfo Info;
typedef _STL::pair<unsigned long, Info> Entry;
typedef CVectorMap<unsigned long, Info, CKeyPairCompareLess<unsigned long, Info> > Map;

char g_FableEmptyNarrowString_0129AAF4[1];
static const void* g_crcData;
static fable_u32 g_crcLength;
static fable_u32 g_crcValue;
fable_u32 FABLE_FASTCALL FableCRC_Calc_004014A0(fable_u32 seed, const void* data, fable_u32 byteCount)
{
    g_crcData = data;
    g_crcLength = byteCount;
    return seed == 0 ? g_crcValue : 0;
}

static const Map* g_lbMap;
static int g_lbCalls;
template<> Entry* Map::LowerBound(const unsigned long& key)
{
    ++g_lbCalls;
    g_lbMap = this;
    Entry* it = this->_M_start;
    while (it != this->_M_finish && it->first < key)
        ++it;
    return it;
}

struct Probe : CDefinitionManager
{
    Map& Classes() { return DefClassInfo; }
    Info* Get(const CCharString& name) { return GetPDefClassInfo(name); }
};

static int Check(Probe* m, const CCharString& name, fable_u32 crc, Info* want)
{
    g_crcValue = crc;
    g_lbCalls = 0;
    Info* got = m->Get(name);
    if (got != want || g_lbCalls != 1 || g_lbMap != &m->Classes())
    {
        printf("crc %lx: got %p want %p (calls %d)\n", (unsigned long)crc, (void*)got, (void*)want, g_lbCalls);
        return 1;
    }
    return 0;
}

int main()
{
    static unsigned char storage[sizeof(Probe)];
    memset(storage, 0, sizeof(storage));
    Probe* m = reinterpret_cast<Probe*>(storage);

    static unsigned char entryStore[3][sizeof(Entry)];
    memset(entryStore, 0, sizeof(entryStore));
    Entry* entries = reinterpret_cast<Entry*>(entryStore);
    entries[0].first = 10;
    entries[1].first = 20;
    entries[2].first = 30;
    m->Classes()._M_start = entries;
    m->Classes()._M_finish = entries + 3;

    CCharStringData data;
    data.Data.PData = (char*)"FRONT_END";
    data.Data.StringLength = 9;
    unsigned char nameStore[sizeof(CCharString)];
    CCharString* name = reinterpret_cast<CCharString*>(nameStore);
    *reinterpret_cast<CCharStringData**>(name) = &data;

    if (Check(m, *name, 20, &entries[1].second)) return 1;
    if (g_crcData != data.Data.PData || g_crcLength != 9) { puts("crc input"); return 1; }
    if (Check(m, *name, 10, &entries[0].second)) return 1;
    if (Check(m, *name, 30, &entries[2].second)) return 1;
    if (Check(m, *name, 15, 0)) return 1;   // lower bound is a larger key
    if (Check(m, *name, 5, 0)) return 1;
    if (Check(m, *name, 31, 0)) return 1;   // lower bound is the end

    // An empty (null-data) name hashes the shared empty buffer, length 0.
    unsigned char emptyStore[sizeof(CCharString)];
    CCharString* empty = reinterpret_cast<CCharString*>(emptyStore);
    *reinterpret_cast<CCharStringData**>(empty) = 0;
    if (Check(m, *empty, 30, &entries[2].second)) return 1;
    if (g_crcData != g_FableEmptyNarrowString_0129AAF4 || g_crcLength != 0) { puts("empty crc input"); return 1; }

    // An empty map finds nothing.
    m->Classes()._M_finish = entries;
    if (Check(m, *name, 10, 0)) return 1;
    puts("FABLETLC_CDEFINITIONMANAGER_GETPDEFCLASSINFO_009AD580_PASS");
    return 0;
}