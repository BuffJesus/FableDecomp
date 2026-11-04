#pragma once
// PDB member names from Ego_r (0x38 with its 16-byte maps). Retail uses
// three 12-byte STLport maps: constructor 0x009F0320, sizeof 0x2C.
// Retail RTTI at vtable 0x0129C764 confirms the CTBaseSingleton base.
#include "fable_string.h"
#include "fable_stlport_tree.h"
#include "fable_console.h"
class CCodeSectionManager : public CTBaseSingleton<CCodeSectionManager> {
public:
    struct CSectionDesc {
        CCharString ShortName;
        CCharString LongName;
        bool Loaded;
        long Size;
    };
    // Ego_r.pdb CCodeSectionManager::CDebugInfo: same layout as CSectionDesc
    // (GetDebugInfoByIndex 0x009F0160 returns one by value).
    struct CDebugInfo {
        CCharString ShortName;
        CCharString LongName;
        bool Loaded;
        long Size;
    };
    CCodeSectionManager();
    virtual ~CCodeSectionManager();
    void LoadSection(unsigned long handle);
    void SetEnableSectionLoading(bool enable);
    long GetSectionsRegisteredCount();
    CDebugInfo GetDebugInfoByIndex(long index);
    _STL::map<CCharString, unsigned long> ShortNameToHandle;
    _STL::map<CCharString, unsigned long> LongNameToHandle;
    _STL::map<unsigned long, CSectionDesc> SectionDesc;
    bool SectionLoadingEnabled;
private:
    friend class CCodeSectionManagerTestAccess;
    CSectionDesc& GetSectionDesc(unsigned long handle);
};
FABLE_STATIC_ASSERT(sizeof(CCodeSectionManager) == 0x2C);
FABLE_STATIC_ASSERT(sizeof(CCodeSectionManager::CSectionDesc) == 0x10);
FABLE_STATIC_ASSERT(sizeof(CCodeSectionManager::CDebugInfo) == 0x10);
extern CCountedPointer<CCodeSectionManager> g_FableCodeSectionManager_013CAA68;
template<> __forceinline CCodeSectionManager& FABLE_FASTCALL CTBaseSingleton<CCodeSectionManager>::Get() {
    if (g_FableCodeSectionManager_013CAA68.IsNull())
        g_FableCodeSectionManager_013CAA68.Reset(new CCodeSectionManager);
    return *g_FableCodeSectionManager_013CAA68;
}
extern unsigned long g_FableXMVCodeHandle_013CA7E0;
extern unsigned long g_FableXMVCodeReferences_013CA7E4;
void FABLE_FASTCALL StaticLoadXMVCode();

// This find instantiation is out of line in the retail CodeSectionManager unit.
typedef _STL::map<unsigned long, CCodeSectionManager::CSectionDesc>::_Rep_type CCodeSectionTree;
namespace _STL {
    template<> CCodeSectionTree::iterator CCodeSectionTree::find(const unsigned long& key);
}
