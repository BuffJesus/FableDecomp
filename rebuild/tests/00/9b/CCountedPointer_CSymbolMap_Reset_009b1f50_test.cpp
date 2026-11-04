#include "../src/compiled/00/4a/CSymbolMap_ContainerLifetime_004ad4a0.cpp"
// Behaviour of CCountedPointer<CSymbolMap>::Reset(CSymbolMap*) 0x009B1F50:
// the old map is deleted (virtually) only when its last reference goes, its
// info block freed; a new map gets a fresh info block (count 1, the map's
// deleter, the map); Reset(0) leaves the pointer empty.
#include "fable_counted_pointer.h"
#include "fable_symbol_map.h"
#include <new>
#include <stdio.h>
#include <string.h>

// Fixture scaffolding: minimal CSymbolMap lifetime that counts deletes.
static int g_deleted;
static const void* g_deletedThis;
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() { ++g_deleted; g_deletedThis=this; }
CCharString::~CCharString() {}
CSymbolMap::CSymbolMap() {}


int main()
{
    CCountedPointer<CSymbolMap> pointer;
    CSymbolMap* first = new CSymbolMap;
    pointer.Reset(first);
    CCPPointerInfo* info = pointer.Info;
    if (pointer.Data != first || !info || info->RefCount != 1 || info->Data != first || !info->DeleteFunc) { puts("first"); return 1; }

    // Shared by a second owner: resetting drops one reference, deletes nothing.
    ++info->RefCount;
    CSymbolMap* second = new CSymbolMap;
    pointer.Reset(second);
    if (g_deleted != 0 || info->RefCount != 1 || pointer.Data != second || pointer.Info == info) { puts("shared"); return 1; }
    CCPPointerInfo* secondInfo = pointer.Info;
    if (secondInfo->RefCount != 1 || secondInfo->Data != second || secondInfo->DeleteFunc != info->DeleteFunc) { puts("second"); return 1; }

    // Last owner: the map is deleted through its virtual destructor.
    pointer.Reset(0);
    if (g_deleted != 1 || g_deletedThis != second || pointer.Data != 0 || pointer.Info != 0) { puts("last"); return 1; }

    // The deleter tolerates a null map.
    info->DeleteFunc(0);
    info->DeleteFunc(first);
    if (g_deleted != 2 || g_deletedThis != first) { puts("deleter"); return 1; }
    delete info;
    puts("FABLETLC_CCOUNTEDPOINTER_CSYMBOLMAP_RESET_009B1F50_PASS");
    return 0;
}

