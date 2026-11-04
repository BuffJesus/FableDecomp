// Behaviour of CDefinitionManager::GetPDefFromClassIndex 0x009ADE10: the
// named class's GlobalIndices[index] (no offset) picks the instantiated def,
// LoadDef runs on that global index first, and the returned pointer shares
// the def (one lock) or is null when the def is not loaded.
#include "fable_definition_manager.h"
#include <stdio.h>
#include <string.h>

struct FakeDef
{
    virtual void Slot0() {}
    virtual void Delete() const {}
    mutable unsigned long count;
};

static unsigned char g_info[sizeof(CDefinitionManager::CDefClassInfo)];
static const CCharString* g_className;
static unsigned long g_loaded = 0xFFFFFFFF;
static int g_loads;

const CDefinitionManager::CDefClassInfo* CDefinitionManager::GetPDefClassInfo(const CCharString& className) const
{
    g_className = &className;
    return reinterpret_cast<const CDefClassInfo*>(g_info);
}

void CDefinitionManager::LoadDef(unsigned long globalIndex) const
{
    ++g_loads; g_loaded = globalIndex;
}

struct Access : CDefinitionManager
{
    static _STL::vector<CInstantiatedDefInfo*>& Defs(CDefinitionManager* m) { return static_cast<Access*>(m)->InstantiatedDefs; }
};

int main()
{
    static unsigned char store[sizeof(CDefinitionManager)];
    CDefinitionManager* m = reinterpret_cast<CDefinitionManager*>(store);
    unsigned char nameStore[sizeof(CCharString)];
    const CCharString& name = *reinterpret_cast<CCharString*>(nameStore);

    static unsigned long globals[4] = { 7, 2, 0, 3 };
    reinterpret_cast<CDefinitionManager::CDefClassInfo*>(g_info)->GlobalIndices._M_start = globals;

    static unsigned char infoStore[4][sizeof(CDefinitionManager::CInstantiatedDefInfo)];
    static CDefinitionManager::CInstantiatedDefInfo* defs[8];
    for (int i = 0; i < 4; ++i) defs[i] = reinterpret_cast<CDefinitionManager::CInstantiatedDefInfo*>(infoStore[i]);
    Access::Defs(m)._M_start = defs;

    FakeDef a;
    a.count = 1;
    defs[2]->PDef.Object = &a;
    defs[3]->PDef.Object = 0;

    {
        CDefPointer<const CDefClassBase> p = m->GetPDefFromClassIndex(name, 1);
        if (g_className != &name || g_loads != 1 || g_loaded != 2) { puts("lookup"); return 1; }
        if (p.Object != &a || a.count != 2) { puts("shared"); return 1; }
    }
    if (a.count != 1) { puts("unlocked"); return 1; }

    {
        CDefPointer<const CDefClassBase> p = m->GetPDefFromClassIndex(name, 3);
        if (g_loads != 2 || g_loaded != 3 || p.Object != 0) { puts("unloaded def"); return 1; }
    }

    puts("FABLETLC_CDEFINITIONMANAGER_GETPDEFFROMCLASSINDEX_009ADE10_PASS");
    return 0;
}