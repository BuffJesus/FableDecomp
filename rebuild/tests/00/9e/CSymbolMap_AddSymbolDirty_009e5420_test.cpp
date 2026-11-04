// Behaviour of CSymbolMap::AddSymbolDirty(const CCharString&, long) 0x009E5420:
// with CheckForDuplicateSymbols the name goes into NamesUsed first; the symbol
// is appended to SLONGMap as (name, {CurrentFilename, name, val}) and the map
// marked dirty. CurrentFilename starts as "NULL".
#include "fable_symbol_map.h"
#include <stdio.h>
#include <string.h>

// Fixture scaffolding: CCharString copies share the source's data pointer, so
// the appended pair shows where each string came from.
fable_i32 g_CCharStringInstanceCount_013BD800;
CCharString::CCharString(const char* text, long) { *reinterpret_cast<const char**>(this) = text; }
CCharString::CCharString(const CCharString& other) { *reinterpret_cast<void**>(this) = *reinterpret_cast<void* const*>(&other); }
CCharString::~CCharString() {}

typedef _STL::pair<CCharString, CSymbolMap::CSymbolInfo> Entry;
static int g_inserts;
static const void* g_insertThis;
static const CCharString* g_insertKey;
template<> _STL::pair<_STL::_Rb_tree_iterator<CCharString>, bool>
_STL::_Rb_tree<CCharString, CCharString, _STL::_Identity<CCharString>, _STL::less<CCharString>, _STL::allocator<CCharString> >::insert_unique(const CCharString& key)
{
    ++g_inserts;
    g_insertThis = this;
    g_insertKey = &key;
    _STL::_Rb_tree_iterator<CCharString> it((_STL::_Rb_tree_node_base*)0);
    return _STL::pair<_STL::_Rb_tree_iterator<CCharString>, bool>(it, true);
}
static int g_pushes;
static const void* g_pushThis;
static const void* g_first;
static const void* g_group;
static const void* g_entry;
static long g_val;
template<> void _STL::vector<Entry, _STL::allocator<Entry> >::push_back(const Entry& e)
{
    ++g_pushes;
    g_pushThis = this;
    g_first = *reinterpret_cast<void* const*>(&e.first);
    g_group = *reinterpret_cast<void* const*>(&e.second.GroupName);
    g_entry = *reinterpret_cast<void* const*>(&e.second.EntryName);
    g_val = e.second.Val;
}

int main()
{
    static unsigned char storage[sizeof(CSymbolMap)];
    CSymbolMap* map = reinterpret_cast<CSymbolMap*>(storage);
    const char* nameText = "FRONT_END";
    CCharString name(nameText, -1);

    map->AddSymbolDirty(name, 7);
    if (g_inserts != 0 || g_pushes != 1 || g_pushThis != storage + 4) { puts("no check"); return 1; }
    if (g_first != nameText || g_entry != nameText || g_val != 7 || !g_group || strcmp((const char*)g_group, "NULL") != 0) { puts("entry"); return 1; }
    if (storage[0x11] != 1) { puts("dirty"); return 1; }

    storage[0x20] = 1;
    map->AddSymbolDirty(name, -2);
    if (g_inserts != 1 || g_insertThis != storage + 0x14 || g_insertKey != &name || g_pushes != 2 || g_val != -2) { puts("check"); return 1; }
    puts("FABLETLC_CSYMBOLMAP_ADDSYMBOLDIRTY_009E5420_PASS");
    return 0;
}
