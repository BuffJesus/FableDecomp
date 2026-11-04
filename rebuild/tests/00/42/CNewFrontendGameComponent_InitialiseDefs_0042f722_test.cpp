#include "fable_game.h"
#include "fable_definition_manager.h"
#include "test_support/resource_list_stubs.h"
void CIVCountedPointeeBase::DeleteCIVCountedPointeeBase() const {}
#include <stdio.h>
#include <string.h>
static int step;
static bool ok;
static void* expectedOutput;
static CCharString* expectedName;
// CStartupDefinitionManager now declares only its retail ctor (0x00433693,
// takes the .bin name), so the singleton the stub Get returns is raw storage.
static unsigned long managerStorage[sizeof(CStartupDefinitionManager) / 4];
static CStartupDefinitionManager& manager =
    *reinterpret_cast<CStartupDefinitionManager*>(managerStorage);
// CDefinitionManager is polymorphic (retail vtable 0x0123117C); the static
// instance above emits its vtable, so its virtual members need bodies here.
CDefinitionManager::~CDefinitionManager() {}
CWideString::~CWideString() {}
void CStartupDefinitionManager::InitAndCompile() { ok = false; }
static CCharStringData textData;
void FABLE_FASTCALL CNewFrontendGameComponent::CompileDefs() { ok = ok && ++step == 1; }
CCharString::CCharString(const char* text, long length) : storage_(&textData) {
    ok = ok && ++step == 2 && strcmp(text, "FRONT_END") == 0 && length == -1;
    expectedName = this;
}
CCharString::~CCharString() { ok = ok && ++step == 5 && this == expectedName; }
CStartupDefinitionManager& FABLE_FASTCALL CStartupDefinitionManager::Get() {
    ok = ok && ++step == 3;
    return manager;
}
template<> bool CDefinitionManager::GetDef<CFrontEndDef>(
    const CCharString& name, long index, CDefPointer<const CFrontEndDef>* output) const {
    ok = ok && ++step == 4 && this == &manager && &name == expectedName &&
        index == 1 && output == expectedOutput;
    output->Object = reinterpret_cast<void*>(0x12345678);
    return true;
}
int main() {
    unsigned long storage[sizeof(CNewFrontendGameComponent)/4];
    unsigned char expected[sizeof(storage)];
    memset(storage, 0xA5, sizeof(storage));
    memcpy(expected, storage, sizeof(storage));
    unsigned long value = 0x12345678;
    memcpy(expected + 0x3C, &value, sizeof(value));
    expectedOutput = reinterpret_cast<unsigned char*>(storage) + 0x3C;
    ok = true;
    reinterpret_cast<CNewFrontendGameComponent*>(storage)->InitialiseDefs();
    if (!ok || step != 5 || memcmp(expected, storage, sizeof(storage))) return 1;
    puts("PASS_0042f722");
    return 0;
}
