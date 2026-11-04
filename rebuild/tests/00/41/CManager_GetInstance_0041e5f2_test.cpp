#include <stdio.h>
#include <string.h>

#include <stdlib.h>
#include "fable_nui_manager.h"

static int g_newCalls;
static size_t g_newSize;
static bool g_newFails;
void* operator new(size_t size)
{
    ++g_newCalls;
    g_newSize = size;
    return g_newFails ? 0 : malloc(size);
}
void operator delete(void* p) { free(p); }

// Behaviour fixture for NUISystem::CManager::GetInstance @ 0x0041E5F2: the
// first call allocates and constructs the singleton (GPUniqueInstance),
// later calls return it without constructing again.

NUISystem::CManager* NUISystem::CManager::GPUniqueInstance = 0;

static int g_ctorCalls;
static void* g_ctorThis;

// Out-of-line virtuals so the stand-in ctor below has a vtable to store.
void NUISystem::CObservable::SetEvent(EEvent) {}
void NUISystem::CObservable::SetOwnedEvent(CObserver*, EEvent) {}
void NUISystem::CObservable::AddObserver(CObserver*) {}
void NUISystem::CObservable::AddExclusiveObserver(CObserver*) {}
void NUISystem::CObservable::RemoveExclusiveObserver(CObserver*) {}
void NUISystem::CObservable::RemoveObserver(CObserver*) {}
void NUISystem::CObservable::ClearObservers() {}
NUISystem::CObserver* NUISystem::CObservable::GetExclusiveObserver() { return 0; }
bool NUISystem::CObservable::IsExclusivelyObserved() { return false; }
void NUISystem::CObservable::AddConcurrentExclusiveObserver(CObserver*) {}
void NUISystem::CObservable::RemoveConcurrentExclusiveObserver(CObserver*) {}
bool NUISystem::CManager::GetMouseDisabled() { return false; }
void NUISystem::CManager::SetKeyPressed(wchar_t) {}
wchar_t NUISystem::CManager::GetKeyPressed() { return 0; }

NUISystem::CManager::CManager()
{
    ++g_ctorCalls;
    g_ctorThis = this;
    m_bHasErrorMessage = true;   // marks the object as ours
}

int main()
{
    bool ok = true;
    // Allocation failure: nothing constructed, null cached, null returned.
    g_newFails = true;
    ok = ok && NUISystem::CManager::GetInstance() == 0 && g_ctorCalls == 0 && g_newCalls == 1;
    g_newFails = false;
    NUISystem::CManager* first = NUISystem::CManager::GetInstance();
    ok = ok && first != 0 && g_ctorCalls == 1 && g_ctorThis == first;
    ok = ok && g_newCalls == 2 && g_newSize == 0xD0;
    ok = ok && first->HasErrorMessage();
    ok = ok && NUISystem::CManager::GetInstance() == first && g_ctorCalls == 1 && g_newCalls == 2;
    ok = ok && NUISystem::CManager::GetInstance() == first && g_ctorCalls == 1 && g_newCalls == 2;
    if (ok) printf("FABLETLC_NUI_CMANAGER_GETINSTANCE_PASS\n");
    else printf("FAIL ctor=%d\n", g_ctorCalls);
    return ok ? 0 : 1;
}
