// Behaviour of CSymbolMap::CSymbolMap() 0x009E5200: the CBaseClassNonCopyable
// base first, then an empty SLONGMap (null vector, Dirty false), an empty
// NamesUsed set (a malloc'd header node linked to itself, count 0) and
// CheckForDuplicateSymbols false; the object ends on the CSymbolMap vtable.
#include "fable_symbol_map.h"
#include <new>
#include <stdio.h>
#include <string.h>

// Recording stand-ins (fixture scaffolding) for the base class.
static int g_baseCalls;
static const void* g_baseThis;
static int g_destroyed;
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() { ++g_baseCalls; g_baseThis = this; }
CBaseClassNonCopyable::~CBaseClassNonCopyable() { ++g_destroyed; }

CCharString::~CCharString() {}

static unsigned long U32(const unsigned char* base, unsigned int offset)
{
    return *reinterpret_cast<const unsigned long*>(base + offset);
}

int main()
{
    static unsigned char storage[sizeof(CSymbolMap)];
    memset(storage, 0xA5, sizeof(storage));
    CSymbolMap* map = new (storage) CSymbolMap();
    if ((void*)map != storage || g_baseCalls != 1 || g_baseThis != storage) { puts("base"); return 1; }
    if (U32(storage, 4) != 0 || U32(storage, 8) != 0 || U32(storage, 12) != 0 || storage[0x11] != 0) { puts("SLONGMap"); return 1; }
    const unsigned char* header = reinterpret_cast<const unsigned char*>(U32(storage, 0x14));
    if (!header || header[0] != 0 || U32(header, 4) != 0 || U32(header, 8) != (unsigned long)header ||
        U32(header, 12) != (unsigned long)header || U32(storage, 0x18) != 0) { puts("NamesUsed"); return 1; }
    if (storage[0x20] != 0) { puts("CheckForDuplicateSymbols"); return 1; }
    CBaseClass* base = map;
    base->~CBaseClass();
    if (g_destroyed != 1) { puts("vtable"); return 1; }
    puts("FABLETLC_CSYMBOLMAP_CTOR_009E5200_PASS");
    return 0;
}

#include "../src/compiled/00/4a/CSymbolMap_ContainerLifetime_004ad4a0.cpp"
