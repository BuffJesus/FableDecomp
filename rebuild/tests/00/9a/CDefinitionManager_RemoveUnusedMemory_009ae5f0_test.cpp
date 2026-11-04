#include <stdio.h>
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
typedef CVectorMap<CCharString, CSymbolMap::CSymbolInfo,
                   CCompareString<CSymbolMap::CSymbolInfo> > SymbolStorage;
typedef CDefinitionManager::CInstantiatedDefInfo InstantiatedInfo;

static char trace[32];
static unsigned traceSize;
static void* expectInstantiated;
static void* expectClasses[2];
static void* expectSymbolStorage;
static void* expectChildMap;
static CDefClassBase* firstDef;
static CDefClassBase* secondDef;
static bool valid = true;

static void record(char c) { trace[traceSize++] = c; }

template<> void CArrayVector<InstantiatedInfo*>::DeallocateUnusedMemory()
{
    record('I');
    valid = valid && this == expectInstantiated;
}
template<> void CArrayVector<unsigned long>::DeallocateUnusedMemory()
{
    record(this == expectClasses[0] ? '0' : '1');
    valid = valid && (this == expectClasses[0] || this == expectClasses[1]);
}
template<> void SymbolStorage::DeallocateUnusedMemory()
{
    record('S');
    valid = valid && this == expectSymbolStorage;
}
#ifndef FABLE_GATE_COMPOSED
void CSymbolMap::DeallocateUnusedMemory()
{
    SLONGMap.DeallocateUnusedMemory();
}
#endif
template<> void ChildMap::DeallocateUnusedMemory()
{
    record('C');
    valid = valid && this == expectChildMap;
}

static void* __fastcall virtualChildren(CDefClassBase* self, void*)
{
    record('V');
    valid = valid && (self == firstDef || self == secondDef);
    return self == firstDef ? expectChildMap : 0;
}

static void setVector(unsigned char* where, void* start, void* finish)
{
    memcpy(where, &start, sizeof(start));
    memcpy(where + 4, &finish, sizeof(finish));
    memcpy(where + 8, &finish, sizeof(finish));
}

int main()
{
    unsigned char managerBytes[sizeof(ProbeManager)] = {0};
    unsigned char symbolBytes[sizeof(CSymbolMap)] = {0};
    unsigned char classes[2 * 24] = {0};
    unsigned char instantiatedA[16] = {0};
    unsigned char instantiatedB[16] = {0};
    unsigned char definitionA[sizeof(CDefClassBase)] = {0};
    unsigned char definitionB[sizeof(CDefClassBase)] = {0};
    unsigned char childBytes[sizeof(ChildMap)] = {0};
    void* vtable[25] = {0};
    vtable[24] = reinterpret_cast<void*>(&virtualChildren);
    void* vtablePointer = vtable;
    memcpy(definitionA, &vtablePointer, 4);
    memcpy(definitionB, &vtablePointer, 4);
    firstDef = reinterpret_cast<CDefClassBase*>(definitionA);
    secondDef = reinterpret_cast<CDefClassBase*>(definitionB);
    memcpy(instantiatedA + 12, &firstDef, 4);
    memcpy(instantiatedB + 12, &secondDef, 4);
    InstantiatedInfo* entries[2] = {
        reinterpret_cast<InstantiatedInfo*>(instantiatedA),
        reinterpret_cast<InstantiatedInfo*>(instantiatedB)
    };
    setVector(managerBytes + 0xA4, entries, entries + 2);
    setVector(managerBytes + 0x78, classes, classes + sizeof(classes));
    CSymbolMap* symbolPointer = reinterpret_cast<CSymbolMap*>(symbolBytes);
    memcpy(managerBytes + 0x60, &symbolPointer, 4);
    expectInstantiated = managerBytes + 0xA4;
    expectClasses[0] = classes + 12;
    expectClasses[1] = classes + 36;
    expectSymbolStorage = symbolBytes + 4;
    expectChildMap = childBytes;

    reinterpret_cast<ProbeManager*>(managerBytes)->RunShrink();
    trace[traceSize] = 0;
    if (!valid || strcmp(trace, "IS01VVCV") != 0) return 1;
    puts("DEFINITION_REMOVE_UNUSED_MEMORY_PASS");
    return 0;
}
