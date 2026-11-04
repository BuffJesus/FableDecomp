#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_definition_manager.h"
#include "fable_def_class_base.h"
#include "fable_symbol_map.h"

class ProbeManager : public CDefinitionManager
{
public:
    virtual void InitAndCompile() {}
    void RunShrink() { RemoveUnusedMemory(); }
};

typedef CVectorMap<unsigned long, CDefClassBase::CSubDefInfo,
                   CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> > ChildMap;
static ChildMap* expectedChild;
static unsigned virtualCalls;

static void* __fastcall virtualChildren(CDefClassBase*, void*)
{
    ++virtualCalls;
    return expectedChild;
}

static void setVector(unsigned char* where, void* start, void* finish, void* capacity)
{
    memcpy(where, &start, 4);
    memcpy(where + 4, &finish, 4);
    memcpy(where + 8, &capacity, 4);
}

static void* field(unsigned char* where, unsigned offset)
{
    void* value;
    memcpy(&value, where + offset, 4);
    return value;
}

int main()
{
    unsigned char managerBytes[sizeof(ProbeManager)] = {0};
    unsigned char symbolBytes[sizeof(CSymbolMap)] = {0};
    unsigned char classEntry[24] = {0};
    unsigned char instantiated[16] = {0};
    unsigned char definition[sizeof(CDefClassBase)] = {0};
    unsigned char childBytes[sizeof(ChildMap)] = {0};
    unsigned long* indices = static_cast<unsigned long*>(malloc(2 * sizeof(unsigned long)));
    CDefinitionManager::CInstantiatedDefInfo** entries =
        static_cast<CDefinitionManager::CInstantiatedDefInfo**>(malloc(2 * sizeof(void*)));
    unsigned long* children = static_cast<unsigned long*>(malloc(2 * 3 * sizeof(unsigned long)));
    if (!indices || !entries || !children) return 1;
    indices[0] = 27;
    setVector(classEntry + 12, indices, indices + 1, indices + 2);
    setVector(managerBytes + 0x78, classEntry, classEntry + 24, classEntry + 24);

    void* vtable[25] = {0};
    vtable[24] = reinterpret_cast<void*>(&virtualChildren);
    void* vtablePointer = vtable;
    memcpy(definition, &vtablePointer, 4);
    CDefClassBase* def = reinterpret_cast<CDefClassBase*>(definition);
    memcpy(instantiated + 12, &def, 4);
    entries[0] = reinterpret_cast<CDefinitionManager::CInstantiatedDefInfo*>(instantiated);
    CDefinitionManager::CInstantiatedDefInfo* expectedEntry = entries[0];
    setVector(managerBytes + 0xA4, entries, entries + 1, entries + 2);

    children[0] = 91;
    children[1] = 92;
    children[2] = 93;
    setVector(childBytes, children, children + 3, children + 6);
    expectedChild = reinterpret_cast<ChildMap*>(childBytes);
    CSymbolMap* symbols = reinterpret_cast<CSymbolMap*>(symbolBytes);
    memcpy(managerBytes + 0x60, &symbols, 4);

    ProbeManager& manager = *reinterpret_cast<ProbeManager*>(managerBytes);
    manager.RunShrink();
    if (virtualCalls != 2) return 2;
    if (field(managerBytes + 0xA4, 4) != field(managerBytes + 0xA4, 8) ||
        *static_cast<CDefinitionManager::CInstantiatedDefInfo**>(field(managerBytes + 0xA4, 0)) != expectedEntry) return 3;
    if (field(classEntry + 12, 4) != field(classEntry + 12, 8) ||
        *static_cast<unsigned long*>(field(classEntry + 12, 0)) != 27) return 4;
    if (field(childBytes, 4) != field(childBytes, 8)) return 5;
    unsigned long* copied = static_cast<unsigned long*>(field(childBytes, 0));
    if (copied[0] != 91 || copied[1] != 92 || copied[2] != 93) return 6;
    manager.RunShrink();
    if (virtualCalls != 4) return 7;
    puts("DEFINITION_SHRINK_FULL_PASS");
    return 0;
}
