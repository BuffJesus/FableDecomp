// Behaviour of CDefStringTable::AddString(const CCharString&) 0x009D4F90:
// an empty (null or zero-length) string is -1 with nothing touched. A locked
// table unlocks first. A string already in the map returns its index and
// counts a share. A new string: the empty stream first gets a 4-byte zero
// header; the index is the stream length before the entry; the entry is the
// string's CRC (4 bytes) then the string, NUL-terminated; the map records the
// index, the string count goes up and the global text pointer is the stream
// data + 4.
#include "fable_def_string.h"
#include <stdio.h>
#include <string.h>

typedef _STL::map<CCharString, long> Map;
static char g_log[512];
static void Log(const char* t) { strcat(g_log, t); }
char g_FableEmptyNarrowString_0129AAF4[1];

static fable_u32 g_crcSeed; static const void* g_crcData; static fable_u32 g_crcLen;
fable_u32 FABLE_FASTCALL FableCRC_Calc_004014A0(fable_u32 seed, const void* data, fable_u32 n)
{
    g_crcSeed = seed; g_crcData = data; g_crcLen = n; return 0xC0DEF00D;
}

static CDefStringTable* g_table;
void CDefStringTable::Unlock() { if (this == g_table) Log("unlock;"); }

// The map: one known string (by address), found as a node whose value is 77.
static const CCharString* g_known;
static _STL::_Rb_tree_node<Map::value_type>* g_knownNode;
static _STL::_Rb_tree_node_base* g_header;
template<> Map::_Rep_type::_Base_ptr Map::_Rep_type::_M_find(const CCharString& k)
{
    Log("find;");
    if (&k == g_known) return g_knownNode;
    return g_header;
}
static long g_slot = -5;
static const CCharString* g_indexKey;
template<> long& Map::operator[](const CCharString& k) { Log("index;"); g_indexKey = &k; return g_slot; }

// The stream (a hand-built vtable: slot 4 Write).
static unsigned long g_length;
static unsigned char g_bytes[64];
unsigned long CMemoryDataOutputStream::GetLength() const { return g_length; }
void CDataOutputStream::WriteRepeatedByte(unsigned char value, long count)
{
    char t[32]; sprintf(t, "repeat%d,%ld;", value, count); Log(t);
    memset(g_bytes + g_length, value, count); g_length += count;
}
static void FABLE_FASTCALL FakeWrite(void* self, void*, const void* data, long size)
{
    char t[32]; sprintf(t, "write%ld;", size); Log(t);
    memcpy(g_bytes + g_length, data, size); g_length += size;
}
static const CCharString* g_written;
void CDataOutputStream::WriteNullTerminatedString(const CCharString& text) { Log("text;"); g_written = &text; g_length += 6; }
const void* CMemoryDataOutputStream::PeekData() const { return g_bytes; }

int main()
{
    static unsigned char storage[sizeof(CDefStringTable)];
    memset(storage, 0, sizeof(storage));
    CDefStringTable* table = g_table = reinterpret_cast<CDefStringTable*>(storage);
    static void* vtable[5] = { 0, 0, 0, 0, (void*)&FakeWrite };
    *reinterpret_cast<void***>(storage + 0x0C) = vtable;
    static _STL::_Rb_tree_node_base header;
    static unsigned char nodeStore[sizeof(_STL::_Rb_tree_node<Map::value_type>)];
    g_header = &header;
    g_knownNode = reinterpret_cast<_STL::_Rb_tree_node<Map::value_type>*>(nodeStore);
    g_knownNode->_M_value_field.second = 77;
    *reinterpret_cast<_STL::_Rb_tree_node_base**>(storage) = &header;

    CCharStringData data;
    data.Data.PData = (char*)"FRONT_END";
    data.Data.StringLength = 9;
    CCharStringData emptyData;
    emptyData.Data.PData = (char*)"";
    emptyData.Data.StringLength = 0;
    static unsigned char s[4][sizeof(CCharString)];
    CCharString* nullText = reinterpret_cast<CCharString*>(s[0]);
    CCharString* emptyText = reinterpret_cast<CCharString*>(s[1]);
    CCharString* known = reinterpret_cast<CCharString*>(s[2]);
    CCharString* fresh = reinterpret_cast<CCharString*>(s[3]);
    *reinterpret_cast<void**>(nullText) = 0;
    *reinterpret_cast<void**>(emptyText) = &emptyData;
    *reinterpret_cast<void**>(known) = &data;
    *reinterpret_cast<void**>(fresh) = &data;
    g_known = known;

    table->Locked = true;
    if (table->AddString(*nullText) != -1 || table->AddString(*emptyText) != -1 || g_log[0]) { puts("empty"); return 1; }

    // Shared: unlock (locked table), find, count the share.
    if (table->AddString(*known) != 77 || strcmp(g_log, "unlock;find;") != 0 || table->SharedCount != 1 ||
        table->StringCount != 0) { printf("shared: %s\n", g_log); return 1; }

    // New, empty stream: header, then CRC + text at offset 4.
    g_log[0] = 0;
    table->Locked = false;
    if (table->AddString(*fresh) != 4) { puts("fresh index"); return 1; }
    if (strcmp(g_log, "find;repeat0,4;write4;text;index;") != 0) { printf("fresh: %s\n", g_log); return 1; }
    unsigned long crc; memcpy(&crc, g_bytes + 4, 4);
    if (crc != 0xC0DEF00D || g_crcSeed != 0 || g_crcData != data.Data.PData || g_crcLen != 9) { puts("crc"); return 1; }
    if (g_written != fresh || g_indexKey != fresh || g_slot != 4 || table->StringCount != 1 ||
        g_DefStringTableText_013CA824 != (const char*)g_bytes + 4) { puts("record"); return 1; }

    // New, stream already has entries: no second header.
    g_log[0] = 0;
    if (table->AddString(*fresh) != 14 || strcmp(g_log, "find;write4;text;index;") != 0 || g_slot != 14 ||
        table->StringCount != 2 || table->SharedCount != 1) { printf("second: %s\n", g_log); return 1; }
    puts("FABLETLC_CDEFSTRINGTABLE_ADDSTRING_009D4F90_PASS");
    return 0;
}