#include "fable_game_text_bank.h"
#include "test_support/resource_list_stubs.h"
#include <stdio.h>
#include <stdlib.h>
static int destroys, frees, finishedCalls;
static bool finished;
static unsigned long asyncStorage[64];
CCharString::CCharString() : storage_(0) {}
CCharString::~CCharString() {}
CWideString::CWideString() : storage_(0) {}
CWideString::~CWideString() {}
bool CBankFileAsyncData::IsFinished() const {
    if (this != reinterpret_cast<CBankFileAsyncData*>(asyncStorage)) abort();
    ++finishedCalls;
    return finished;
}
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d\n",__LINE__); return false; } } while(0)

struct ProbeText : NGameText::CTextEntry {
    virtual ~ProbeText() { ++destroys; }
    static void* operator new(size_t size) { return malloc(size); }
    static void operator delete(void* data) { ++frees; free(data); }
};
static bool CheckText() {
    const int before = destroys;
    NGameText::CDataBankTextEntry owner;
    ProbeText* entry = new ProbeText;
    entry->Owner = &owner;
    owner.PText.data_ = entry;
    CResource* resource = entry;
    CHECK(resource->UnloadResource());
    CHECK(owner.PText.IsNull() && destroys == before+1 && frees == destroys);

    entry = new ProbeText;
    entry->Owner = &owner;
    entry->PFileData.Data = reinterpret_cast<CBankFileAsyncData*>(asyncStorage);
    owner.PText.data_ = entry;
    finished = false;
    const int calls = finishedCalls;
    resource = entry;
    CHECK(resource->UnloadResource());
    CHECK(owner.PText.data_ == entry && entry->IVRefCount == 1 && destroys == before+1);
    CHECK(finishedCalls == calls+1);
    finished = true;
    entry->AddRef();
    CHECK(resource->UnloadResource());
    CHECK(owner.PText.IsNull() && entry->IVRefCount == 1 && destroys == before+1);
    CHECK(finishedCalls == calls+2);
    entry->Release();
    CHECK(destroys == before+2 && frees == destroys);

    entry = new ProbeText;
    entry->Owner = &owner;
    resource = entry;
    CHECK(resource->UnloadResource());
    CHECK(owner.PText.IsNull() && entry->IVRefCount == 1 && destroys == before+2);
    entry->Release();
    CHECK(destroys == before+3 && frees == destroys);
    return true;
}

struct ProbeGroup : NGameText::CGroupEntry {
    virtual ~ProbeGroup() { ++destroys; }
    static void* operator new(size_t size) { return malloc(size); }
    static void operator delete(void* data) { ++frees; free(data); }
};
static bool CheckGroup() {
    const int before = destroys;
    NGameText::CDataBankGroupEntry owner;
    ProbeGroup* entry = new ProbeGroup;
    entry->Owner = &owner;
    owner.PGroup.data_ = entry;
    CResource* resource = entry;
    CHECK(resource->UnloadResource());
    CHECK(owner.PGroup.IsNull() && destroys == before+1 && frees == destroys);

    entry = new ProbeGroup;
    entry->Owner = &owner;
    entry->PFileData.Data = reinterpret_cast<CBankFileAsyncData*>(asyncStorage);
    owner.PGroup.data_ = entry;
    finished = false;
    const int calls = finishedCalls;
    resource = entry;
    CHECK(resource->UnloadResource());
    CHECK(owner.PGroup.data_ == entry && entry->IVRefCount == 1 && destroys == before+1);
    CHECK(finishedCalls == calls+1);
    finished = true;
    entry->AddRef();
    CHECK(resource->UnloadResource());
    CHECK(owner.PGroup.IsNull() && entry->IVRefCount == 1 && destroys == before+1);
    CHECK(finishedCalls == calls+2);
    entry->Release();
    CHECK(destroys == before+2 && frees == destroys);

    entry = new ProbeGroup;
    entry->Owner = &owner;
    resource = entry;
    CHECK(resource->UnloadResource());
    CHECK(owner.PGroup.IsNull() && entry->IVRefCount == 1 && destroys == before+2);
    entry->Release();
    CHECK(destroys == before+3 && frees == destroys);
    return true;
}
int main() {
    if (!CheckText() || !CheckGroup()) return 1;
    printf("TEXT_RESOURCE_UNLOAD_PASS pending=2 lastOwner=2 sharedOwner=2 emptyOwner=2 destroys=%d frees=%d\n", destroys, frees);
    return 0;
}

#include "../../src/compiled/00/42/CIVCountedPointeeBase_DeleteCIVCountedPointeeBaseUBEXXZ_0042d930.cpp"
