// Behaviour fixture for CDefinitionManager::ClearInstantiatedDefs (0x009AD950).
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fable_definition_manager.h"

namespace
{
    int g_deletes;
    void* g_deleted[8];
    int g_pointeeDeletes;
    const void* g_pointeeDeleted;
}

void operator delete(void* p)
{
    if (g_deletes < 8)
        g_deleted[g_deletes] = p;
    ++g_deletes;
    free(p);
}

void CIVCountedPointeeBase::DeleteCIVCountedPointeeBase() const
{
    ++g_pointeeDeletes;
    g_pointeeDeleted = this;
}

struct RawVector
{
    void* start;
    void* finish;
    void* endOfStorage;
};

struct ManagerView : CDefinitionManager
{
    static RawVector& Defs(CDefinitionManager* m) { return *reinterpret_cast<RawVector*>(&static_cast<ManagerView*>(m)->InstantiatedDefs); }
    static RawVector& Classes(CDefinitionManager* m) { return *reinterpret_cast<RawVector*>(&static_cast<ManagerView*>(m)->DefClassInfo); }
    static void Clear(CDefinitionManager* m) { static_cast<ManagerView*>(m)->ClearInstantiatedDefs(); }
};

int main()
{
    static fable_u8 managerBytes[sizeof(CDefinitionManager)];
    memset(managerBytes, 0, sizeof(managerBytes));
    CDefinitionManager* manager = reinterpret_cast<CDefinitionManager*>(managerBytes);

    CIVCountedPointeeBase shared;     // another holder keeps it alive
    shared.IVRefCount = 2;
    CIVCountedPointeeBase last;       // this record holds the last reference
    last.IVRefCount = 1;

    CDefinitionManager::CInstantiatedDefInfo* infos[3];
    infos[0] = new CDefinitionManager::CInstantiatedDefInfo;
    infos[0]->PDef.Reset(reinterpret_cast<CDefClassBase*>(&shared));
    infos[1] = 0;
    infos[2] = new CDefinitionManager::CInstantiatedDefInfo;
    infos[2]->PDef.Reset(reinterpret_cast<CDefClassBase*>(&last));
    CDefinitionManager::CInstantiatedDefInfo* first = infos[0];
    CDefinitionManager::CInstantiatedDefInfo* third = infos[2];
    RawVector& defs = ManagerView::Defs(manager);
    defs.start = infos;
    defs.finish = infos + 3;
    defs.endOfStorage = infos + 3;

    // Two def classes with global index lists of 2 and 0 entries.
    unsigned long indices[4] = { 5, 9, 0, 0 };
    struct Entry { unsigned long crc; void* name; void* alloc; unsigned long* s; unsigned long* f; unsigned long* e; };
    Entry entries[2] = { { 1, 0, 0, indices, indices + 2, indices + 4 }, { 2, 0, 0, 0, 0, 0 } };
    RawVector& classes = ManagerView::Classes(manager);
    classes.start = entries;
    classes.finish = entries + 2;
    classes.endOfStorage = entries + 2;

    g_deletes = 0;
    ManagerView::Clear(manager);

    const bool passed =
        infos[0] == 0 && infos[1] == 0 && infos[2] == 0 &&
        g_deletes == 2 && g_deleted[0] == first && g_deleted[1] == third &&
        shared.IVRefCount == 1 && last.IVRefCount == 0 &&
        g_pointeeDeletes == 1 && g_pointeeDeleted == &last &&
        entries[0].f == indices && entries[0].e == indices + 4 && entries[1].f == 0 &&
        defs.finish == infos + 3;
    shared.IVRefCount = 1;
    if (!passed)
    {
        printf("FABLETLC_CDEFINITIONMANAGER_CLEARINSTANTIATEDDEFS_009AD950 FAIL\n");
        return 1;
    }
    printf("FABLETLC_CDEFINITIONMANAGER_CLEARINSTANTIATEDDEFS_009AD950_PASS\n");
    return 0;
}
