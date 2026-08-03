#include <stdio.h>
#include <string.h>

struct CDefClassBase
{
    long pad0;
    long RefCount;
};

struct CDefTableEntry
{
    long pad0;
    long pad1;
    long pad2;
    CDefClassBase* pDef;
};

struct CDefPointer_CDefClassBase
{
    CDefClassBase* Ptr;

    CDefPointer_CDefClassBase(CDefClassBase* p) : Ptr(p)
    {
        if (Ptr)
            ++Ptr->RefCount;
    }
};

struct CDefinitionManager
{
    char pad[0xa4];
    CDefTableEntry** GlobalIndexTable;

    void LoadDef(unsigned long globalIndex);
    CDefPointer_CDefClassBase GetPDefFromGlobalIndex_b(long globalIndex);
};

static int           g_loadDefHit = 0;
static unsigned long g_loadDefArg = 0;
void CDefinitionManager::LoadDef(unsigned long globalIndex)
{
    g_loadDefHit = 1;
    g_loadDefArg = globalIndex;
}

CDefPointer_CDefClassBase CDefinitionManager::GetPDefFromGlobalIndex_b(long globalIndex)
{
    LoadDef((unsigned long)globalIndex);
    return CDefPointer_CDefClassBase(GlobalIndexTable[globalIndex]->pDef);
}

int main(void)
{
    int ok = 1;

    // ---- non-null def: pointer copied out, refcount incremented ----
    CDefClassBase def;
    memset(&def, 0, sizeof(def));
    def.RefCount = 5;

    CDefTableEntry entry;
    memset(&entry, 0, sizeof(entry));
    entry.pDef = &def;

    CDefTableEntry* table[4];
    table[2] = &entry;

    CDefinitionManager mgr;
    memset(&mgr, 0, sizeof(mgr));
    mgr.GlobalIndexTable = table;

    g_loadDefHit = 0; g_loadDefArg = 0;
    CDefPointer_CDefClassBase r = mgr.GetPDefFromGlobalIndex_b(2);

    if (!g_loadDefHit)              { printf("FAIL loaddef not called\n"); ok = 0; }
    if (g_loadDefArg != 2)          { printf("FAIL loaddef arg %lu\n", g_loadDefArg); ok = 0; }
    if (r.Ptr != &def)              { printf("FAIL ptr\n"); ok = 0; }
    if (def.RefCount != 6)          { printf("FAIL refcount %ld\n", def.RefCount); ok = 0; }

    // ---- null def: pointer copied as null, no refcount touch (no crash) ----
    CDefTableEntry entry2;
    memset(&entry2, 0, sizeof(entry2));
    entry2.pDef = 0;

    CDefTableEntry* table2[4];
    table2[1] = &entry2;
    mgr.GlobalIndexTable = table2;

    g_loadDefHit = 0; g_loadDefArg = 0;
    CDefPointer_CDefClassBase r2 = mgr.GetPDefFromGlobalIndex_b(1);

    if (!g_loadDefHit)              { printf("FAIL2 loaddef not called\n"); ok = 0; }
    if (g_loadDefArg != 1)          { printf("FAIL2 loaddef arg %lu\n", g_loadDefArg); ok = 0; }
    if (r2.Ptr != 0)                { printf("FAIL2 ptr not null\n"); ok = 0; }

    if (ok) printf("GETPDEFFROMGLOBALINDEX_B_009ADA10_TEST PASS\n");
    else    printf("GETPDEFFROMGLOBALINDEX_B_009ADA10_TEST FAIL\n");
    return ok ? 0 : 1;
}