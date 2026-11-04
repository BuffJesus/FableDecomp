// Behaviour of CFrontEndDef::Alloc() 0x00433080: operator new(0x54); when
// that fails, returns null without constructing. Otherwise runs CResource's
// ctor (count 1, off every list), clears PDefManager and the Setup /
// Template / DefaultValsApplied bits (other bits of that byte kept), leaves
// GlobalIndex alone, sets an unnamed InstantiationName (-1), an empty clean
// sub-def map and an empty attract-movie list, installs the CFrontEndDef
// vtable (its own GetSizeofClass / Transfer, the parent-def IsParentDef) and
// leaves the three graphic ids uninitialised.
#include "fable_frontend_def.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static unsigned char g_heap[0x60];
static size_t g_newSize;
static bool g_newFails;
void* operator new(size_t size)
{
    g_newSize = size;
    if (g_newFails) return 0;
    memset(g_heap, 0xA5, sizeof(g_heap));
    return g_heap;
}
void operator delete(void*) {}

static int g_resourceCtors;
CResource::CResource()
    : ResourceList(0), PrevResource(0), NextResource(0), ResourceSize(0), LastUsedFrame(0)
{
    ++g_resourceCtors;
}
void CResourceList::RemoveResource(CResource&) {}
void CIVCountedPointeeBase::DeleteCIVCountedPointeeBase() const {}
CCharString::~CCharString() {}

// The vtable's other slots (never called here); slots 3-9 are CResource's
// inline defaults.
bool CDefClassBase::UnloadResource() { return false; }
CDefString CDefClassBase::GetInstantiationName() const { return CDefString(); }
void CDefClassBase::SetInstantiationName(CDefString) {}
void CDefClassBase::AddSubDef(const CCharString&, long, long) {}
void CDefClassBase::RemoveSubDef(const CCharString&) {}
const CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&) const { return 0; }
CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&) { return 0; }
bool CDefClassBase::IsParentDef() const { return false; }
void CDefClassBase::PostTransfer() {}
void CDefClassBase::InternalSaveBinary(CDataOutputStream&) {}
void CDefClassBase::InternalLoadBinary(CDataInputStream&) {}
void CDefClassBase::Validate(const CValidateDefinitionInitBase&, const CCharString&) const {}
void* CDefClassBase::GetPSubDefInfoMap() { return 0; }
CDefString CParentDefClassBase::GetInstantiationName() const { return InstantiationName; }
void CParentDefClassBase::SetInstantiationName(CDefString name) { InstantiationName = name; }
void CParentDefClassBase::AddSubDef(const CCharString&, long, long) {}
void CParentDefClassBase::RemoveSubDef(const CCharString&) {}
const CDefClassBase::CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString&) const { return 0; }
CDefClassBase::CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString&) { return 0; }
bool CParentDefClassBase::IsParentDef() const { return true; }
void CParentDefClassBase::InternalSaveBinary(CDataOutputStream&) {}
void CParentDefClassBase::InternalLoadBinary(CDataInputStream&) {}
void* CParentDefClassBase::GetPSubDefInfoMap() { return &SubDefInfoMap; }
static int g_transfers;
void CFrontEndDef::Transfer(CPersistContext&) { ++g_transfers; }
void CFrontEndDef::Copy(const CDefClassBase*) {}
unsigned long CFrontEndDef::GetSizeofClass() const { return 0x54; }

static unsigned long U32(int offset) { unsigned long v; memcpy(&v, g_heap + offset, 4); return v; }

int main()
{
    CDefClassBase* def = CFrontEndDef::Alloc();
    if ((void*)def != g_heap || g_newSize != 0x54 || g_resourceCtors != 1) { puts("alloc"); return 1; }
    if (U32(4) != 1 || U32(8) != 0 || U32(0xC) != 0 || U32(0x10) != 0 || U32(0x14) != 0 || U32(0x18) != 0) { puts("resource"); return 1; }
    if (U32(0x1C) != 0 || U32(0x20) != 0xA5A5A5A5) { puts("manager/index"); return 1; }
    if (g_heap[0x24] != (0xA5 & ~7)) { printf("flags %02x\n", g_heap[0x24]); return 1; }
    if (U32(0x25) != 0xFFFFFFFF) { puts("instantiation name"); return 1; }
    if (U32(0x29) != 0 || U32(0x2D) != 0 || U32(0x31) != 0 || g_heap[0x36] != 0) { puts("sub-def map"); return 1; }
    if (U32(0x3C) != 0 || U32(0x40) != 0 || U32(0x44) != 0) { puts("attract movies"); return 1; }
    if (U32(0x48) != 0xA5A5A5A5 || U32(0x4C) != 0xA5A5A5A5 || U32(0x50) != 0xA5A5A5A5) { puts("graphics"); return 1; }
    if (def->GetSizeofClass() != 0x54 || !def->IsParentDef()) { puts("vtable"); return 1; }
    char context[64];
    def->Transfer(*reinterpret_cast<CPersistContext*>(context));
    if (g_transfers != 1) { puts("transfer slot"); return 1; }

    g_newFails = true;
    if (CFrontEndDef::Alloc() != 0 || g_resourceCtors != 1) { puts("failed allocation"); return 1; }
    puts("FABLETLC_CFRONTENDDEF_ALLOC_00433080_PASS");
    return 0;
}