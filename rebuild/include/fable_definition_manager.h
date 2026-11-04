#pragma once
#include "fable_string.h"
#include "fable_counted_pointer.h"
#include "fable_def_pointer.h"
#include "fable_def_string.h"
#include "fable_resource_list.h"
#include "fable_stlport_tree.h"
#include "fable_symbol_map.h"
#include "fable_vector_map.h"
#include "fable_carray.h"
#include "fable_engine_def.h"
template<class T> class CDefPointer;
class CDefClassBase;
class CWideString;
class CMemoryDataInputStream;
class CDataInputStream;
class CDataOutputStream;
class CStringParser;

// FableWin ??0CDefClassInit@@QAE@ABVCCharString@@P6IPAVCDefClassBase@@XZ@Z /
// ??1CDefClassInit@@QAE@XZ; Ego_r layout (8 bytes): Name +0x00, AllocFunc +0x04.
// Both are inline in retail: CompileDefsPlatformSpecific 0x0042F5A9 copies the
// name (CCharString copy ctor 0x0099EC30) then stores the allocator.
class CDefClassInit
{
public:
    typedef CDefClassBase* (FABLE_FASTCALL *AllocFunction)();
    CDefClassInit(const CCharString& name, AllocFunction allocFunc)
        : Name(name), AllocFunc(allocFunc)
    {
    }

    CCharString Name;
    AllocFunction AllocFunc;
};

// Retail layout (sizeof 0xD0; Ego_r says 0xF0 because its MS vectors, maps
// and CArrays are 4 bytes larger), from the ctor 0x009B0470:
//   +0x00 CResourceList base (vtable 0x0129B28C), +0x3C PDefTypeDebugInfoMap,
//   +0x40/+0x4C path vectors, +0x58/+0x60 symbol maps, +0x68/+0x78 vector
//   maps, +0x88 compressed data, +0x94 compressed offsets, +0xA4 instantiated
//   defs, +0xB0 ErrorCallback, +0xB4 CurrentFilename, +0xB8 CompiledFileName,
//   +0xBC DependencyCheckCRC, +0xC0 UseSafeBinaryFormat, +0xC4 RecompiledDefs.
// Ego_r's CArray<T> members are 12-byte STLport vectors in retail; they are
// declared as _STL::vector here because fable_game_text_bank.h's CArray
// still carries the 16-byte Ego_r shape.
class CDefinitionManager : public CResourceList
{
public:
    // Vtable 0x0129B28C: 0 scalar deleting destructor 0x009B9270, 1 the
    // inherited CResourceList::HandleFailedAllocation 0x009FC540, 2
    // InitAndCompile (pure: slot is _purecall 0x00BFEA08; CGameDefinition-
    // Manager's is 0x0044C72B). CMainGameComponent::InitDefinitions
    // 0x00416005 calls slot 2 through [vptr+8].
    virtual ~CDefinitionManager();
    virtual void InitAndCompile() = 0;

    typedef void (FABLE_FASTCALL *ErrorCallbackFunction)(const CCharString& message);
    // FableWin ??0CDefinitionManager@@QAE@ABVCWideString@@P6IXABVCCharString@@@Z@Z,
    // retail 0x009B0470 (the .bin file name and the compiler's error callback;
    // callers: CStartupDefinitionManager 0x00433693, CGameDefinitionManager 0x0044C6C2).
    CDefinitionManager(const CWideString& filename, ErrorCallbackFunction errorCallback);

    // FableWin ?ClearSymbols@CDefinitionManager@@QAEXXZ (retail 0x009ACB10;
    // caller CMainGameComponent::InitDefinitions 0x00416005).
    void ClearSymbols();

    // FableWin const / mutable global-index overloads; retail 009AD9E0 / 009ADA10.
    // FableWin const name lookup, retail 009AD410.
    long GetDefGlobalIndexFromName(const CCharString& name) const;
    // FableWin const/mutable instantiation-name lookup, retail 009ADA40 / 009ADA90.
    CDefPointer<const CDefClassBase> GetPDefFromInstantiationName(const CCharString& name) const;
    CDefPointer<CDefClassBase> GetPDefFromInstantiationName(const CCharString& name);
    CDefPointer<const CDefClassBase> GetPDefFromGlobalIndex(long globalIndex) const;
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long globalIndex);

    template<class T> bool GetDef(long index, CDefPointer<const T>* output) const;
    template<class T> bool GetDef(const CCharString& name, CDefPointer<const T>* output) const;
    template<class T> bool GetDef(const CCharString& name, long index,
        CDefPointer<const T>* output) const;
    // FableWin ?GetPDefFromClassIndex@CDefinitionManager@@QBE?AV?$CDefPointer@$$CBVCDefClassBase@@@@ABVCCharString@@J@Z,
    // retail 0x009ADE10 (the manifest's "GetPDefFromGlobalIndex" label is wrong):
    // the def of the named class at a class-local index, returned in a hidden
    // result slot (GetDef<T> 0x004300FB reuses its dead index argument slot).
    CDefPointer<const CDefClassBase> GetPDefFromClassIndex(const CCharString& className, long index) const;
    // FableWin ?ScrambleDefs@CDefinitionManager@@QAEX_N@Z: the only one-bool
    // void member; empty in retail (0x009AC9E0, `ret 4`, just before Error
    // 0x009AC9F0). Its only caller is CMainGameComponent::Run 0x004189C2.
    void ScrambleDefs(bool scramble);

    // FableWin ?SetSymbolPathList@CDefinitionManager@@QAEXABV?$vector@VCWideString@@...@Z
    // and ?SetCompilePathList@...; inline in retail (InitAndCompilePlatformSpecific
    // 0x004336BC assigns through vector<CWideString>::operator= 0x00433DF0).
    void SetSymbolPathList(const _STL::vector<CWideString>& symbolPathList)
    {
        SymbolPathList = symbolPathList;
    }
    void SetCompilePathList(const _STL::vector<CWideString>& compilePathList)
    {
        CompilePathList = compilePathList;
    }
    // FableWin ?Compile@CDefinitionManager@@QAEXXZ, retail 0x009B08C0: load the
    // compiled defs, or compile the path lists' sources.
    void Compile();
    // FableWin ?LoadBinaryDefinitions@CDefinitionManager@@QAE_NAAVCDataInputStream@@@Z,
    // retail 0x009AFB90; ?SaveBinaryDefinitions@...@@QAEXAAVCDataOutputStream@@@Z,
    // retail 0x009AF7A0 (both called by Compile).
    bool LoadBinaryDefinitions(CDataInputStream& stream);
    void SaveBinaryDefinitions(CDataOutputStream& stream);
    // FableWin ?SetCreatingCompiledDefs@CDefinitionManager@@SIX_N@Z, retail
    // 0x004036D0 (stores 0x0138E188; GFMain sets it). Compile writes the
    // compiled file when it is set.
    static void FABLE_FASTCALL SetCreatingCompiledDefs(bool creating);

    // FableWin ?AddDefClass@CDefinitionManager@@QAEXABVCDefClassInit@@@Z,
    // retail 0x009B0AC0 (caller CNewFrontendGameComponent::CompileDefsPlatformSpecific).
    void AddDefClass(const CDefClassInit& init);

    // FableWin ?SetCompiledDefDirectory@CDefinitionManager@@SIXABVCWideString@@@Z
    // (retail 0x004036B0: CompiledDefDirectory = directory).
    static void FABLE_FASTCALL SetCompiledDefDirectory(const CWideString& directory);

    // Ego_r CDefinitionManager::CDefClassInfo (retail: 20 bytes).
    class CDefClassInfo
    {
    public:
        CCharString Name;
        CDefClassInit::AllocFunction AllocFunc;
        _STL::vector<unsigned long> GlobalIndices;   // Ego_r CArray<unsigned long>
    };
    // Ego_r CDefinitionManager::CInstantiatedDefInfo (16 bytes): two CDefString
    // names (4 bytes each, not modelled), the class index and the def itself
    // (+0x0C; AddDefClass 0x009B0AC0 copies it out of InstantiatedDefs).
    class CInstantiatedDefInfo
    {
    public:
        // FableWin ??0CInstantiatedDefInfo@CDefinitionManager@@QAE@VCDefString@@0KABV?$CDefPointer@VCDefClassBase@@@@@Z;
        // inline in retail (InstantiateDef 0x009ADC90).
        // FableWin ??0CInstantiatedDefInfo@CDefinitionManager@@QAE@XZ; inline
        // in retail (LoadBinaryDefinitions 0x009AFB90: both names -1, no def).
        CInstantiatedDefInfo()
        {
        }
        CInstantiatedDefInfo(CDefString className, CDefString instantiationName,
                             unsigned long classIndex, const CDefPointer<CDefClassBase>& pdef)
            : ClassName(className), InstantiationName(instantiationName), ClassIndex(classIndex), PDef(pdef)
        {
        }

        CDefString ClassName;
        CDefString InstantiationName;
        unsigned long ClassIndex;
        CDefPointer<CDefClassBase> PDef;
    };

protected:
    // FableWin ?GetPDefClassInfo@CDefinitionManager@@IAEPAVCDefClassInfo@1@ABVCCharString@@@Z,
    // retail 0x009AD580 (InstantiateDef 0x009ADC90 calls it; it goes through the
    // non-const LowerBound 0x009B2230). 0x009AD2E0 is a byte-identical body
    // over the other LowerBound (0x009B2290), presumably the const overload
    // ?GetPDefClassInfo@CDefinitionManager@@IBEPBVCDefClassInfo@1@ABVCCharString@@@Z;
    // AddDefClass 0x009B0AC0 calls that one.
    CDefClassInfo* GetPDefClassInfo(const CCharString& className);
    const CDefClassInfo* GetPDefClassInfo(const CCharString& className) const;
    // FableWin ?GetPDefClassInfo@CDefinitionManager@@IAEPAVCDefClassInfo@1@K@Z
    // (by class-name CRC); inline in retail (LoadBinaryDefinitions 0x009AFB90).
    CDefClassInfo* GetPDefClassInfo(unsigned long classCRC)
    {
        _STL::pair<unsigned long, CDefClassInfo>* it = DefClassInfo.Find(classCRC);
        if (it == DefClassInfo.end())
            return 0;
        return &it->second;
    }
    // FableWin ?DecompressDefinitionData@CDefinitionManager@@IBEXAAVCMemoryDataInputStream@@K@Z,
    // retail 0x009AD5F0: opens the stream over the def's decompressed data.
    void DecompressDefinitionData(CMemoryDataInputStream& stream, unsigned long globalIndex) const;
    // FableWin ?InstantiateDef@CDefinitionManager@@IAE_NABVCCharString@@0_N@Z,
    // retail 0x009ADC90.
    bool InstantiateDef(const CCharString& className, const CCharString& instantiationName, bool flag);
    // FableWin ?LoadDef@CDefinitionManager@@IBEXK@Z, retail 0x009AD6E0.
    void LoadDef(unsigned long globalIndex) const;
    // Compile 0x009B08C0's steps (FableWin protected members):
    // ?CheckDependencies@...@@IAEXABV?$vector@VCWideString@@...@Z 0x009B05F0,
    // ?CreateSymbolsFromPathList@...@@IAEXABV?$vector@...@_N@Z 0x009AEF60,
    // ?DoCompilePathList@...@@IAEXABV?$vector@...@ABVCCharString@@@Z 0x009AF480,
    // ?RemoveUnusedMemory@CDefinitionManager@@IAEXXZ 0x009AE5F0.
    void CheckDependencies(const _STL::vector<CWideString>& pathList);
    void CreateSymbolsFromPathList(const _STL::vector<CWideString>& pathList, bool flag);
    void DoCompilePathList(const _STL::vector<CWideString>& pathList, const CCharString& prefix);
    void RemoveUnusedMemory();
    // FableWin ?ParseStringForSymbols@CDefinitionManager@@IAEXPBD_N@Z,
    // retail 0x009ADE90; CreateSymbolsFromPathList calls it for each sorted file.
    void ParseStringForSymbols(const char* source, bool flag);
    void ParseEnumForSymbols(CStringParser& parser);
    void Error(const CStringParser& parser, const CCharString& message);
    // Donor CompileString; retail 0x009AF1E0, called by DoCompilePathList.
    void CompileString(const char* source, const CCharString& typeFilter);
    void CompileDefinition(CStringParser& parser, const CCharString& className, const CCharString& instanceName);
    void CompileSubDefinition(CCharString source, const CCharString& name, long globalIndex);
    CDefPointer<CDefClassBase> InstantiateSubDef(const CCharString& name, unsigned long parentIndex);
    // FableWin ?ClearInstantiatedDefs@CDefinitionManager@@IAEXXZ, retail 0x009AD950.
    void ClearInstantiatedDefs();
    void CompressDefinitionBlock(CMemoryDataOutputStream& data,
        CArrayVector<unsigned short>& offsets, unsigned long firstGlobalIndex);
    // Ego_r ?UnloadDef@CDefinitionManager@@IAE_NK@Z: release the instantiated
    // def unless something beyond the manager and its caller still holds it.
    // Retail has no standalone copy; CDefClassBase::UnloadResource 0x009ACE90
    // expands it (CDefClassBase reaches it as a friend).
    bool UnloadDef(unsigned long globalIndex)
    {
        CInstantiatedDefInfo* info = InstantiatedDefs[globalIndex];
        if (info->PDef.GetRefCount() <= 2)
        {
            info->PDef.Reset(0);
            return true;
        }
        return false;
    }
    friend class CDefClassBase;

    // ?CompiledDefDirectory@CDefinitionManager@@1VCWideString@@A (FableWin:
    // protected static), retail 0x013CA7DC; the ctor prefixes it to the file name.
    static CWideString CompiledDefDirectory;
    // Ego_r static members, set at boot by SetUsingCompiledDefs 0x004036C0
    // (retail 0x013CA7D8) and SetCreateChecksums 0x004036F0 (retail 0x0138E189)
    // from GFMain; AddDefClass 0x009B0AC0 builds a class's NULLDEF_ default
    // when checksumming or compiling from text.
    static bool UsingCompiledDefs;
    static bool CreateChecksums;
    // Ego_r static member, retail 0x0138E188 (SetCreatingCompiledDefs).
    static bool CreatingCompiledDefs;

    void* PDefTypeDebugInfoMap;                            // +0x3C Ego_r map<CCharString,CDefTypeDebugInfo>*; not set by the ctor
    _STL::vector<CWideString> SymbolPathList;              // +0x40
    _STL::vector<CWideString> CompilePathList;             // +0x4C
    CCountedPointer<CSymbolMap> PSymbolMap;                // +0x58
    CCountedPointer<CSymbolMap> PDefinitionSymbolMap;      // +0x60
    CVectorMap<unsigned long, long,
        CKeyPairCompareLess<unsigned long, long> > NameCRCToIndexMap;          // +0x68
    CVectorMap<unsigned long, CDefClassInfo,
        CKeyPairCompareLess<unsigned long, CDefClassInfo> > DefClassInfo;      // +0x78
    CArrayVector<unsigned char> CompressedDefData;         // +0x88 Ego_r CArray<unsigned char>
    CVectorMap<unsigned long, unsigned long,
        CKeyPairCompareLess<unsigned long, unsigned long> > CompressedOffsets; // +0x94
    _STL::vector<CInstantiatedDefInfo*> InstantiatedDefs;  // +0xA4 Ego_r CArray<CInstantiatedDefInfo*>
    ErrorCallbackFunction ErrorCallback;                   // +0xB0
    CCharString CurrentFilename;                           // +0xB4
    CWideString CompiledFileName;                          // +0xB8
    unsigned long DependencyCheckCRC;                      // +0xBC
    bool UseSafeBinaryFormat;                              // +0xC0
    _STL::map<unsigned long, CDefPointer<CDefClassBase> > RecompiledDefs;  // +0xC4
};
FABLE_STATIC_ASSERT(sizeof(CDefinitionManager) == 0xD0);

// The front end's definitions come from the startup manager, not the game one:
// Get 0x0043368D and Set 0x004336B0 (Reset of its singleton at 0x013B8760,
// RTTI vtable 0x0123117C). Callers: CNewFrontendGameComponent::InitialiseDefs
// 0x0042F722 and ::Shutdown 0x0042EBB6.
class CStartupDefinitionManager : public CDefinitionManager
{
public:
    static CStartupDefinitionManager& FABLE_FASTCALL Get();
    static void FABLE_FASTCALL Set(CStartupDefinitionManager* manager);

    // FableWin ??0CStartupDefinitionManager@@QAE@ABVCWideString@@@Z, retail
    // 0x00433693 (operator new(0xD0) in CompileDefsPlatformSpecific 0x0042F5A9).
    CStartupDefinitionManager(const CWideString& filename);
    // FableWin ?InitAndCompilePlatformSpecific@CStartupDefinitionManager@@QAEX_N@Z,
    // retail 0x004336BC.
    void InitAndCompilePlatformSpecific(bool xbox);

    // FableWin ?InitAndCompile@CStartupDefinitionManager@@UAEXXZ: an empty
    // `ret` in retail (0x0043368B, vtable 0x0123117C slot 2).
    virtual void InitAndCompile();

protected:
    // FableWin ?ErrorCallback@CStartupDefinitionManager@@KIXABVCCharString@@@Z
    // (protected static): an empty `ret` in retail, 0x0043368C, handed to the
    // CDefinitionManager ctor by CStartupDefinitionManager 0x00433693.
    static void FABLE_FASTCALL ErrorCallback(const CCharString& message);

    // ?PClass@CStartupDefinitionManager@@1V?$CCountedPointer@VCStartupDefinitionManager@@@@A
    // (FableWin: protected static), retail 0x013B8760. Static storage only.
    static CCountedPointer<CStartupDefinitionManager> PClass;

    // Retail sizeof is 0xD0 (operator new(0xD0) at 0x0042F5D3): no members
    // of its own beyond CDefinitionManager.
};
FABLE_STATIC_ASSERT(sizeof(CStartupDefinitionManager) == 0xD0);

// Def classes the front end registers with the startup manager. Only their
// static allocators (FableWin ?Alloc@<class>@@SIPAVCDefClassBase@@XZ) are
// referenced here, by address, from CompileDefsPlatformSpecific 0x0042F5A9.
class CControlsDef
{
public:
    // retail 0x00432180
    static CDefClassBase* FABLE_FASTCALL Alloc();
};
// The other startup def classes, registered by CStartupDefinitionManager::
// InitAndCompilePlatformSpecific 0x004336BC (Ego_r names its allocators).
class CEngineVideoOptionsDef
{
public:
    // retail 0x00434540
    static CDefClassBase* FABLE_FASTCALL Alloc();
};
class CConfigOptionsDefaultsDef
{
public:
    // retail 0x00434660
    static CDefClassBase* FABLE_FASTCALL Alloc();
};
namespace NUISystem
{
class CUIDef
{
public:
    // retail 0x00433670
    static CDefClassBase* FABLE_FASTCALL Alloc();
};
class CUIMiscThingsDef
{
public:
    // retail 0x00432C40
    static CDefClassBase* FABLE_FASTCALL Alloc();
};
class CUIIconsDef
{
public:
    // retail 0x00432E00
    static CDefClassBase* FABLE_FASTCALL Alloc();
};
}

class CGameDefinitionManager : public CDefinitionManager
{
public:
    // GameDefinitionManager.cpp: Get 0x0044C6B0 (`mov eax,[0x013B879C]`) and
    // Set 0x0044C71F (Reset of the singleton at 0x013B879C); ctor 0x0044C6C2,
    // RTTI vtable 0x01232C24.
    static CGameDefinitionManager& FABLE_FASTCALL Get();
    static void FABLE_FASTCALL Set(CGameDefinitionManager* manager);

    // FableWin ?IsAllocated@CGameDefinitionManager@@SI_NXZ, retail 0x0044C6B6.
    static bool FABLE_FASTCALL IsAllocated();
    // FableWin ??0CGameDefinitionManager@@QAE@XZ, retail 0x0044C6C2.
    CGameDefinitionManager();

protected:
    // ?PClass@CGameDefinitionManager@@1V?$CCountedPointer@VCGameDefinitionManager@@@@A
    // (FableWin: protected static), retail 0x013B879C. Static storage only.
    static CCountedPointer<CGameDefinitionManager> PClass;

private:
    // Retail sizeof is 0xE0 (operator new(0xE0) in CMainGameComponent::Init);
    // its own members (after CDefinitionManager's 0xD0) are not modelled.
    fable_u8 unknownD0_[0xE0 - 0xD0];

public:
    // FableWin ?InitAndCompile@CGameDefinitionManager@@UAEXXZ (retail 0x0044C72B).
    virtual void InitAndCompile();
};
FABLE_STATIC_ASSERT(sizeof(CGameDefinitionManager) == 0xE0);
