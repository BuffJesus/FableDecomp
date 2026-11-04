// Behaviour fixture for CParentDefClassBase's instantiation-name accessors,
// IsParentDef, GetPSubDefInfoMap, AddSubDef, both GetPSubDefInfo overloads
// and InternalSaveBinary. The members are the real compiled providers, called
// non-virtually on zeroed def storage. Names are real CCharStrings over
// fixture-built string data; the CRC, the map's binary searches, the stream
// writes and STLport's sort (0x009B84C0, an insertion sort here) are
// fixture-side.
#include <new>
#define private public
#include "fable_string.h"
#undef private
#include "fable_def_class_base.h"
#include "fable_data_output_stream.h"
#include "fable_stlport_vector_impl.h"
#include <stdio.h>
#include <string.h>

typedef CParentDefClassBase::CSubDefInfoMap Map;
typedef Map::value_type Entry;

// ---- fixture-side callees ----------------------------------------------------
fable_u32 FABLE_FASTCALL FableCRC_Calc_004014A0(fable_u32 seed, const void* data, fable_u32 count)
{
    fable_u32 h = seed ^ 0x811c9dc5u;
    for (fable_u32 i = 0; i < count; ++i) h = (h ^ static_cast<const unsigned char*>(data)[i]) * 0x01000193u;
    return h;
}
char g_FableEmptyNarrowString_0129AAF4[1] = { 0 };
template <> Entry* Map::LowerBound(const unsigned long& key)
{
    Entry* it = begin(); while (it != end() && it->first < key) ++it; return it;
}
template <> const Entry* Map::LowerBound(const unsigned long& key) const
{
    const Entry* it = begin(); while (it != end() && it->first < key) ++it; return it;
}
namespace _STL {
template <> void FABLE_FASTCALL sort<Entry*, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> >(
    Entry* first, Entry* last, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo>)
{
    for (Entry* i = first + 1; i < last; ++i)
        for (Entry* j = i; j > first && j->first < (j - 1)->first; --j) swap(*j, *(j - 1));
}
}

static unsigned long g_written[64]; static char g_kinds[64]; static int g_writes;
void CDataOutputStream::WriteUBYTE(unsigned char v) { g_kinds[g_writes] = 'b'; g_written[g_writes++] = v; }
void CDataOutputStream::WriteUWORD(unsigned short v) { g_kinds[g_writes] = 'w'; g_written[g_writes++] = v; }
void CDataOutputStream::WriteULONG(unsigned long v) { g_kinds[g_writes] = 'L'; g_written[g_writes++] = v; }
void CDataOutputStream::WriteSLONG(long v) { g_kinds[g_writes] = 'l'; g_written[g_writes++] = (unsigned long)v; }

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
    unsigned long Crc() const { return FableCRC_Calc_004014A0(0, reinterpret_cast<const CCharStringData*>(data)->GetText_(), (fable_u32)strlen(reinterpret_cast<const CCharStringData*>(data)->GetText_())); }
};
CCharString::CCharString() : storage_(0) {}
CCharString::~CCharString() {}

static int g_fail;
#define CHECK(c, code) do { if (!(c)) { printf("PARENT_DEF_FAIL %d line %d\n", code, __LINE__); g_fail = 1; } } while (0)

int main()
{
    int cases = 0;
    static union { double align; unsigned char bytes[sizeof(CParentDefClassBase)]; } storage;
    CParentDefClassBase* def = reinterpret_cast<CParentDefClassBase*>(&storage);
    // Instantiation name, IsParentDef, map accessor.
    for (long pos = -1; pos < 3; ++pos) {
        memset(&storage, 0, sizeof(storage));
        CDefString name; name.TablePos = pos;
        def->CParentDefClassBase::SetInstantiationName(name);
        CHECK(def->CParentDefClassBase::GetInstantiationName().TablePos == pos && def->InstantiationName.TablePos == pos, 1);
        CHECK(def->CParentDefClassBase::IsParentDef() && def->CParentDefClassBase::GetPSubDefInfoMap() == &def->SubDefInfoMap, 2);
        ++cases;
    }
    static const char* const names[] = { "Sword", "Shield", "Hat", "Boots", "Cloak", "Ring", "Amulet", "Belt" };
    for (int count = 0; count <= 8; ++count) for (int flags = 0; flags < 8; flags += 3) {
        memset(&storage, 0, sizeof(storage));
        def->Setup = flags & 1; def->Template = (flags >> 1) & 1; def->DefaultValsApplied = (flags >> 2) & 1;
        // Add in a scrambled order; the map stays sorted by key and every name is found.
        for (int i = 0; i < count; ++i) {
            int k = (i * 11 + 3) % count;
            Name n(names[k]);
            def->CParentDefClassBase::AddSubDef(n.string, 100 + k, 200 + k);
            CHECK(!def->SubDefInfoMap.Dirty && (long)def->SubDefInfoMap.size() == i + 1, 3);
        }
        for (int i = 1; i < count; ++i) CHECK(def->SubDefInfoMap[i - 1].first <= def->SubDefInfoMap[i].first, 4);
        for (int k = 0; k < count; ++k) {
            Name n(names[k]);
            CDefClassBase::CSubDefInfo* info = def->CParentDefClassBase::GetPSubDefInfo(n.string);
            const CDefClassBase::CSubDefInfo* cinfo = static_cast<const CParentDefClassBase*>(def)->CParentDefClassBase::GetPSubDefInfo(n.string);
            CHECK(info && info == cinfo && info->DefIndex == 100 + k && info->OriginalParentDefIndex == 200 + k, 5);
        }
        Name missing("Missing");
        CHECK(!def->CParentDefClassBase::GetPSubDefInfo(missing.string) && !static_cast<const CParentDefClassBase*>(def)->CParentDefClassBase::GetPSubDefInfo(missing.string), 6);
        // Save: three flag bytes, the 16-bit count, then each (key, index, parent index) in key order.
        unsigned char stream[sizeof(CDataOutputStream) + 16]; memset(stream, 0, sizeof(stream)); g_writes = 0;
        def->SubDefInfoMap.Dirty = count > 1;  // the save sorts first when entries were appended
        if (count > 1) _STL::swap(def->SubDefInfoMap[0], def->SubDefInfoMap[count - 1]);
        def->CParentDefClassBase::InternalSaveBinary(*reinterpret_cast<CDataOutputStream*>(stream));
        CHECK(g_writes == 4 + 3 * count && !def->SubDefInfoMap.Dirty, 7);
        CHECK(g_kinds[0] == 'b' && g_written[0] == (unsigned long)(flags & 1) && g_written[1] == (unsigned long)((flags >> 1) & 1) && g_written[2] == (unsigned long)((flags >> 2) & 1), 8);
        CHECK(g_kinds[3] == 'w' && g_written[3] == (unsigned long)count, 9);
        for (int i = 0; i < count; ++i) {
            CHECK(g_kinds[4 + 3 * i] == 'L' && g_kinds[5 + 3 * i] == 'l' && g_kinds[6 + 3 * i] == 'l', 10);
            CHECK(g_written[4 + 3 * i] == def->SubDefInfoMap[i].first && (long)g_written[5 + 3 * i] == def->SubDefInfoMap[i].second.DefIndex && (long)g_written[6 + 3 * i] == def->SubDefInfoMap[i].second.OriginalParentDefIndex, 11);
            if (i) CHECK(g_written[4 + 3 * (i - 1)] <= g_written[4 + 3 * i], 12);
        }
        ++cases;
    }
    if (g_fail) return 1;
    printf("PARENT_DEF_BEHAVIOR_PASS scenarios=%d\n", cases);
    return 0;
}
