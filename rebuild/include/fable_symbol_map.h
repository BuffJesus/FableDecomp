#pragma once

// CSymbolMap: the definition compiler's name -> value table. Retail sizeof is
// 0x24 (operator new(0x24) twice in CDefinitionManager 0x009B0470; Ego_r says
// 0x2C with MS containers): vptr, SLONGMap (16-byte CVectorMap) +0x04,
// NamesUsed (12-byte STLport set) +0x14, CheckForDuplicateSymbols +0x20.
// RTTI .?AVCSymbolMap@@, vtable 0x0129C158 (slot 0 scalar deleting destructor
// 0x009E7BD0); its ctor 0x009E5200 starts with the CBaseClassNonCopyable ctor
// 0x0099A2F0.

#include "fable_base_class.h"
#include "fable_string.h"
#include "fable_vector_map.h"

// Lionhead's CCharString-keyed comparator (empty): orders map entries by
// CCharString::FastCompare on the key. Inline in retail's sort helpers (its
// out-of-line copy for CSymbolInfo is 0x009E65D0); FastCompare stays a call.
template <class V>
class CCompareString
{
public:
    bool operator()(const _STL::pair<CCharString, V>& a, const _STL::pair<CCharString, V>& b) const
    {
        return a.first.FastCompare(b.first);
    }
};

template<> _STL::set<CCharString>::~set();

class CSymbolMap : public CBaseClassNonCopyable
{
public:
    // Ego_r CSymbolMap::CSymbolInfo (12 bytes).
    class CSymbolInfo
    {
    public:
        // FableWin ??0CSymbolInfo@CSymbolMap@@QAE@ABVCCharString@@0J@Z; inline in retail.
        CSymbolInfo(const CCharString& groupName, const CCharString& entryName, long val)
            : GroupName(groupName), EntryName(entryName), Val(val)
        {
        }

        CCharString GroupName;
        CCharString EntryName;
        long Val;
    };

    // FableWin ??0CSymbolMap@@QAE@XZ, retail 0x009E5200.
    CSymbolMap();
    // The retail destructor is compiler-generated from the two owned containers.

    // FableWin ?AddSymbolDirty@CSymbolMap@@QAEXABVCCharString@@J@Z, retail 0x009E5420:
    // the symbol goes in the group named by CurrentFilename.
    void AddSymbolDirty(const CCharString& name, long val);
    // FableWin ?AddSymbolDirty@CSymbolMap@@QAEXABVCCharString@@0J@Z, retail 0x009E54E0
    // (the two-argument form inlines it).
    void AddSymbolDirty(const CCharString& groupName, const CCharString& name, long val);
    // FableWin ?UpdateDirty@CSymbolMap@@QBEXXZ (const), retail 0x009E5C30.
    void UpdateDirty() const;
    // Ego_r ?UpdateDirtyQuick@CSymbolMap@@QBEXXZ, retail 0x009E5C60: sorts SLONGMap only.
    void UpdateDirtyQuick() const;
    void Clear();
    void DeallocateUnusedMemory();

    // Retail 0x009E5120 and 0x009E51C0; FableWin/PDB names.
    long GetSymbolSLONG(const CCharString& name) const;
    // FableWin CSymbolMap::FindSymbol, const this, output reference; retail 009E5170.
    bool FindSymbol(const CCharString& name, long& value) const;
    bool SymbolExists(const CCharString& name) const;
    // ?FindFirstSymbolWithVal@CSymbolMap@@QBEPBVCSymbolInfo@1@J@Z, retail 009E5090.
    const CSymbolInfo* FindFirstSymbolWithVal(long val) const;
    // ?FindFirstSymbolWithVal@CSymbolMap@@QBEPBVCSymbolInfo@1@ABVCCharString@@J@Z, retail 009E50C0.
    const CSymbolInfo* FindFirstSymbolWithVal(const CCharString& groupName, long val) const;

    // ?CurrentFilename@CSymbolMap@@...: retail 0x013CAA30, the group name new
    // symbols get; "NULL" from startup (dynamic initializer 0x01224170) until
    // a symbol file is being parsed.
    static CCharString CurrentFilename;

    // Ego_r ?LoadFromFile@CSymbolMap@@QAE_NABVCWideString@@@Z, retail 0x009E5C90.
    bool LoadFromFile(const CWideString& name);

protected:
    // Ego_r ?ParseBuffer@CSymbolMap@@IAE_NPBD@Z, retail 0x009E5AD0.
    bool ParseBuffer(const char* text);
    // Ego_r ?ParseEnum@CSymbolMap@@IAE_NAAVCStringParser@@@Z, retail 0x009E55A0.
    bool ParseEnum(class CStringParser& parser);
    // Ego_r ?Error@CSymbolMap@@IAAXABVCWideString@@JPBDZZ, retail 0x009E4F80: builds
    // "file(line) : error C2065 : text" (the message's output is compiled out).
    void __cdecl Error(const CWideString& file, long line, const char* text, ...);

public:

    CVectorMap<CCharString, CSymbolInfo, CCompareString<CSymbolInfo> > SLONGMap;
    _STL::set<CCharString> NamesUsed;
    bool CheckForDuplicateSymbols;
};

template<> CVectorMap<CCharString,CSymbolMap::CSymbolInfo,CCompareString<CSymbolMap::CSymbolInfo> >::~CVectorMap();

FABLE_STATIC_ASSERT(sizeof(CSymbolMap) == 0x24);

// String maps compare the original key by FastCompare, without copying it.
template<> inline const _STL::pair<CCharString,CSymbolMap::CSymbolInfo>*
CVectorMap<CCharString,CSymbolMap::CSymbolInfo,CCompareString<CSymbolMap::CSymbolInfo> >::Find(const CCharString& key) const {
 const _STL::pair<CCharString,CSymbolMap::CSymbolInfo>* it=LowerBound(key);
 const _STL::pair<CCharString,CSymbolMap::CSymbolInfo>* last=this->end();
 if(it==last||key.FastCompare(it->first))return last;
 return it;
}
