#pragma once

// CDefClassBase: the base of every definition class (Ego_r: 0x25 bytes,
// packed). Its CDefPointeeBase part is CResource over CIVCountedPointeeBase
// (0x1C bytes: vptr, reference count at +4, resource-list links), then
// PDefManager +0x1C, GlobalIndex +0x20 and the Setup / Template /
// DefaultValsApplied bits at +0x24.
//
// Retail vtable 0x01230BCC (25 slots, from RTTI against FableWin's): the
// CResource slots 2-9 (UnloadResource is CDefClassBase's own, 0x009ACE90),
// then the CDefClassBase virtuals below; Transfer, Copy and GetSizeofClass
// are pure (_purecall 0x00BFEA08). AddDefClass 0x009B0AC0 calls Transfer
// through [vptr+0x48]; InstantiateDef 0x009ADC90 calls IsParentDef [+0x40]
// and SetInstantiationName [+0x2C].
//
// engine/CDefClassBase.h is the generated flat layout used by a few
// accessor landings; do not include both.

#include "rebuild_abi.h"
#include "fable_def_pointer.h"
#include "fable_def_string.h"
#include "fable_vector_map.h"

class CCharString;
class CDataInputStream;
class CDataOutputStream;
class CDefinitionManager;
class CPersistContext;
class CValidateDefinitionInitBase;

#pragma pack(push, 1)
class CDefClassBase : public CDefPointeeBase
{
public:
    // Ego_r CDefClassBase::CSubDefInfo (8 bytes).
    class CSubDefInfo
    {
    public:
        long DefIndex;
        long OriginalParentDefIndex;
    };

    // FableWin ??0CDefClassBase@@QAE@XZ; inline in retail (the def
    // allocators, e.g. CFrontEndDef::Alloc 0x00433080: no manager yet, the
    // three flags clear; GlobalIndex is left for InstantiateDef).
    CDefClassBase()
        : PDefManager(0)
    {
        Setup = false;
        Template = false;
        DefaultValsApplied = false;
    }

    // CResource slot 2 is CDefClassBase's own (Ego_r); slots 3-9 stay
    // CResource's inline defaults (retail 0x009FBEF0, 0x0042D950..0x0042D980,
    // 0x009FBF00, 0x009FBF10 in every def vtable).
    virtual bool UnloadResource();                                  // [2]  0x009ACE90

    virtual CDefString GetInstantiationName() const;                // [10] 0x0042DA10
    virtual void SetInstantiationName(CDefString name);             // [11] 0x0042DA20
    virtual void AddSubDef(const CCharString& name, long a, long b); // [12] 0x0042DA30
    virtual void RemoveSubDef(const CCharString& name);             // [13] 0x0042DA40
    // VC7.1 gives overloaded virtuals their slots in reverse declaration
    // order, so the non-const overload is declared first (const lands in 14).
    virtual CSubDefInfo* GetPSubDefInfo(const CCharString& name);   // [15] 0x0042DA50
    virtual const CSubDefInfo* GetPSubDefInfo(const CCharString& name) const; // [14] 0x0042DA60
    virtual bool IsParentDef() const;                               // [16] 0x0042DA70
    virtual void PostTransfer();                                    // [17] 0x0042DA80
    virtual void Transfer(CPersistContext& context) = 0;            // [18]
    virtual void Copy(const CDefClassBase* other) = 0;              // [19] FableWin UAEXPBVCDefClassBase@@@Z
    virtual unsigned long GetSizeofClass() const = 0;               // [20]
    virtual void InternalSaveBinary(CDataOutputStream& stream);     // [21] 0x009ACAB0
    virtual void InternalLoadBinary(CDataInputStream& stream);      // [22] 0x009ACB20
    virtual void Validate(const CValidateDefinitionInitBase& init,
                          const CCharString& name) const;           // [23] 0x0042DA90
    virtual void* GetPSubDefInfoMap();                              // [24] 0x0042DAA0 (a CVectorMap*)

    CDefinitionManager* PDefManager;    // +0x1C
    unsigned long GlobalIndex;          // +0x20
    // +0x24 (Ego_r uchar:1 bitfields). FableWin SetAsTemplate(bool) is
    // inline in retail (InstantiateDef 0x009ADC90: the shl/xor/and/xor merge).
    unsigned char Setup : 1;
    unsigned char Template : 1;
    unsigned char DefaultValsApplied : 1;

    // FableWin ?SetAsTemplate@CDefClassBase@@QAEX_N@Z (inline).
    void SetAsTemplate(bool isTemplate) { Template = isTemplate; }
};

// The base of defs that own sub-defs (Ego_r 0x3D bytes; retail 0x39, its
// STLport map being 4 bytes smaller). Its slots override CDefClassBase's
// (retail CFrontEndDef vtable 0x01230FBC: 10 0x004303B0, 11 0x004303C0,
// 12 0x009B00D0, 13 0x009B0170, 14 0x009AD4A0, 15 0x009AD510, 16 0x004303D0,
// 21 0x009B0200, 22 0x009AED70, 24 0x004303E0). Its ctor is inline in
// retail (CFrontEndDef::Alloc 0x00433080).
class CParentDefClassBase : public CDefClassBase
{
public:
    typedef CVectorMap<unsigned long, CSubDefInfo, CKeyPairCompareLess<unsigned long, CSubDefInfo> > CSubDefInfoMap;

    virtual CDefString GetInstantiationName() const;
    virtual void SetInstantiationName(CDefString name);
    virtual void AddSubDef(const CCharString& name, long a, long b);
    virtual void RemoveSubDef(const CCharString& name);
    virtual CSubDefInfo* GetPSubDefInfo(const CCharString& name);
    virtual const CSubDefInfo* GetPSubDefInfo(const CCharString& name) const;
    virtual bool IsParentDef() const;
    virtual void InternalSaveBinary(CDataOutputStream& stream);
    virtual void InternalLoadBinary(CDataInputStream& stream);
    virtual void* GetPSubDefInfoMap();

    CDefString InstantiationName;       // +0x25 (-1 until named)
    CSubDefInfoMap SubDefInfoMap;       // +0x29
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CDefClassBase) == 0x25);
FABLE_STATIC_ASSERT(sizeof(CParentDefClassBase) == 0x39);
