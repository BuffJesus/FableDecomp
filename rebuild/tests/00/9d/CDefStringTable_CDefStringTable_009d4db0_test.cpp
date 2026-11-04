// Behaviour of CDefStringTable::CDefStringTable() 0x009D4DB0 (and the unit's
// GDefStringTable, built before main): an empty string map (a fresh header
// node: red, no parent, linked to itself; no nodes), the string stream and
// file name constructed in place, zero string and share counts, unlocked and
// writable, and RandomID = the table CRC (seed 0) of the 16-byte system time.
#include "fable_def_string.h"
#include "fable_crc.h"
#include "fable_win32_time.h"
#include "test_support/import_redirect.h"
#include <new>
#include <stdio.h>
#include <string.h>
#include "../../../src/compiled/00/43/STL_DefStringMapErase_0043020a.cpp"

fable_u32 g_FableCRCTable_0129A168[256];
static const void* g_streams[4];
static int g_streamCount;
CMemoryDataOutputStream::CMemoryDataOutputStream() { g_streams[g_streamCount++ & 3] = this; }
static const void* g_names[4];
static int g_nameCount;
CWideString::CWideString() { g_names[g_nameCount++ & 3] = this; }
CDefStringTable::~CDefStringTable() {}
// Link-only bodies (the stream's vtable and bases; never run here).
CMemoryDataOutputStream::~CMemoryDataOutputStream() {}

void CMemoryDataOutputStream::SetPosition(unsigned long) {}
unsigned long CMemoryDataOutputStream::GetPosition() const { return 0; }
void CMemoryDataOutputStream::Close() {}
void CMemoryDataOutputStream::Write(const void*, long) {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CCharString::CCharString(const char*, long) {}
CCharString::~CCharString() {}
CWideString::~CWideString() {}

static void FABLE_STDCALL FakeGetSystemTime(_SYSTEMTIME* t)
{
    for (int i = 0; i < 8; ++i) (&t->wYear)[i] = (unsigned short)(0x0101 * (i + 3));
}

int main()
{
    for (unsigned long i = 0; i < 256; ++i) g_FableCRCTable_0129A168[i] = i * 0x9E3779B1UL ^ (i << 7);

    // The global was built at startup.
    const unsigned char* global = reinterpret_cast<const unsigned char*>(&GDefStringTable);
    if (g_streamCount != 1 || g_streams[0] != global + 0x0C || g_nameCount != 1 || g_names[0] != global + 0x30) { puts("global"); return 1; }

    FableTest::ImportRedirect redirect("GetSystemTime", (void*)&FakeGetSystemTime);
    if (!redirect.Installed()) { puts("redirect"); return 1; }
    static unsigned char store[sizeof(CDefStringTable)];
    memset(store, 0xCC, sizeof(store));
    CDefStringTable* table = new (store) CDefStringTable;

    _STL::_Rb_tree_node_base* header = *reinterpret_cast<_STL::_Rb_tree_node_base**>(store);
    if (!header || header->_M_color != false || header->_M_parent != 0 || header->_M_left != header ||
        header->_M_right != header || table->StringMap._M_t._M_node_count != 0) { puts("map"); return 1; }
    if (g_streams[1] != store + 0x0C || g_names[1] != store + 0x30) { puts("members"); return 1; }
    if (table->StringCount != 0 || table->SharedCount != 0 || table->Locked || table->ReadOnly) { puts("fields"); return 1; }

    SYSTEMTIME t;
    FakeGetSystemTime(&t);
    unsigned long want = 0;
    const unsigned char* b = reinterpret_cast<const unsigned char*>(&t);
    for (int i = 0; i < 16; ++i) want = g_FableCRCTable_0129A168[b[i] ^ (want & 0xFF)] ^ (want >> 8);
    if (table->RandomID != want) { printf("random id %08lx want %08lx\n", table->RandomID, want); return 1; }
    puts("FABLETLC_CDEFSTRINGTABLE_CTOR_009D4DB0_PASS");
    return 0;
}
