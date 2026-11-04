// Behaviour fixture for CParentDefClassBase::RemoveSubDef (CVectorMap::
// EraseFromKey) and the sub-def map's UpperBound. The map is filled through
// the real AddSubDef and read back through the real GetPSubDefInfo; the map's
// LowerBound and STLport sort are the landed providers too. The CRC (FNV here,
// so names collide only on purpose via duplicate keys) and the empty string
// are fixture-side; push_back's growth path is instantiated here.
#include <new>
#define private public
#include "fable_string.h"
#undef private
#include "fable_def_class_base.h"
#include "fable_stlport_vector_impl.h"
#include <stdio.h>
#include <string.h>

typedef CParentDefClassBase::CSubDefInfoMap Map;
typedef Map::value_type Entry;

fable_u32 FABLE_FASTCALL FableCRC_Calc_004014A0(fable_u32 seed, const void* data, fable_u32 count)
{
    fable_u32 h = seed ^ 0x811c9dc5u;
    for (fable_u32 i = 0; i < count; ++i) h = (h ^ static_cast<const unsigned char*>(data)[i]) * 0x01000193u;
    return h;
}
char g_FableEmptyNarrowString_0129AAF4[1] = { 0 };

// A CCharString over fixture string data (text pointer +0, length +4).
struct Name
{
    unsigned char data[sizeof(CCharStringData)];
    CCharString string;
    explicit Name(const char* text)
    {
        memset(data, 0, sizeof(data));
        reinterpret_cast<CCharStringData*>(data)->SetText_(const_cast<char*>(text));
        reinterpret_cast<CCharStringData*>(data)->SetLength_((fable_u32)strlen(text));
        string.storage_ = reinterpret_cast<CCharStringData*>(data);
    }
    ~Name() { string.storage_ = 0; }
};
CCharString::CCharString() : storage_(0) {}
CCharString::~CCharString() {}

static int g_fail;
#define CHECK(c, code) do { if (!(c)) { printf("REMOVE_SUBDEF_FAIL %d line %d\n", code, __LINE__); g_fail = 1; } } while (0)

static const char* const kNames[] = { "Sword", "Shield", "Hat", "Boots", "Cloak", "Ring", "Amulet", "Belt", "Ale", "Lute" };
enum { kCount = 10 };

int main()
{
    int cases = 0;
    static union { double align; unsigned char bytes[sizeof(CParentDefClassBase)]; } storage;
    CParentDefClassBase* def = reinterpret_cast<CParentDefClassBase*>(&storage);
    for (int count = 0; count <= kCount; ++count) for (int mask = 0; mask < 8; ++mask)
    {
        memset(&storage, 0, sizeof(storage));
        // Entries 0..count-1, some names twice (a second sub-def under the same key).
        for (int i = 0; i < count; ++i)
        {
            Name n(kNames[(i * 11 + 3) % count]);
            def->CParentDefClassBase::AddSubDef(n.string, i, 100 + i);
            if (((mask >> (i % 3)) & 1) && i % 4 == 0)
                def->CParentDefClassBase::AddSubDef(n.string, 50 + i, 150 + i);
        }
        const long before = (long)def->SubDefInfoMap.size();
        // Remove a selection of present names plus one absent name.
        long removed = 0;
        bool gone[kCount] = { false };
        for (int k = 0; k < count; ++k)
        {
            if (((k + mask) % 3) != 0) continue;
            Name n(kNames[k]);
            long under = 0;
            for (long e = 0; e < (long)def->SubDefInfoMap.size(); ++e) under += def->SubDefInfoMap[e].first == n.string.GetCRC();
            def->CParentDefClassBase::RemoveSubDef(n.string);
            removed += under;
            gone[k] = true;
            CHECK(under > 0 && !def->CParentDefClassBase::GetPSubDefInfo(n.string), 1);
        }
        Name absent("Missing");
        def->CParentDefClassBase::RemoveSubDef(absent.string);
        // Everything else is still there, sorted, and the map is clean.
        CHECK((long)def->SubDefInfoMap.size() == before - removed && !def->SubDefInfoMap.Dirty, 2);
        for (long e = 1; e < (long)def->SubDefInfoMap.size(); ++e) CHECK(def->SubDefInfoMap[e - 1].first <= def->SubDefInfoMap[e].first, 3);
        for (int k = 0; k < count; ++k)
        {
            Name n(kNames[k]);
            const CDefClassBase::CSubDefInfo* info = static_cast<const CParentDefClassBase*>(def)->CParentDefClassBase::GetPSubDefInfo(n.string);
            CHECK(gone[k] ? info == 0 : (info != 0 && info->OriginalParentDefIndex == 100 + info->DefIndex), 4);
        }
        // UpperBound: one past the last entry under each key.
        for (long e = 0; e < (long)def->SubDefInfoMap.size(); ++e)
        {
            const unsigned long key = def->SubDefInfoMap[e].first;
            Entry* upper = def->SubDefInfoMap.UpperBound(key);
            CHECK(upper > &def->SubDefInfoMap[e] && (upper == def->SubDefInfoMap.end() || upper->first > key) && upper[-1].first == key, 5);
        }
        CHECK(def->SubDefInfoMap.UpperBound(0xffffffffu) == def->SubDefInfoMap.end(), 6);
        ++cases;
    }
    if (g_fail) return 1;
    printf("REMOVE_SUBDEF_BEHAVIOR_PASS scenarios=%d\n", cases);
    return 0;
}
