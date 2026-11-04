#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "fable_nui_manager.h"

// Behaviour fixture for NUISystem::CFrontEndManager::GetInstance @ 0x00595582:
// lazily allocates (0xE0 bytes) and constructs the singleton (m_pInstance);
// an allocation failure caches and returns null without constructing.

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

NUISystem::CFrontEndManager* NUISystem::CFrontEndManager::m_pInstance = 0;

CWideString::CWideString() {}
CWideString::~CWideString() {}
NUISystem::CObserver::CObserver() {}
NUISystem::CObserver::~CObserver() {}
CCharString::~CCharString() {}
namespace _STL {
template<class T,class A> list<T,A>::~list(){free(_M_node);}
template<class T,class A> _Deque_base<T,A>::_Deque_base(const A&,size_t){}
template<class T,class A> deque<T,A>::deque():_Deque_base<T,A>(A(),0){}
}
NUISystem::CFrontEndManager::~CFrontEndManager() {}

static int g_ctorCalls;
static void* g_ctorThis;
NUISystem::CFrontEndManager::CFrontEndManager()
{
    ++g_ctorCalls;
    g_ctorThis = this;
    m_bQuitFromGame = true;
}

int main()
{
    bool ok = true;
    g_newFails = true;
    ok = ok && NUISystem::CFrontEndManager::GetInstance() == 0 && g_ctorCalls == 0 && g_newCalls == 1;
    g_newFails = false;
    NUISystem::CFrontEndManager* first = NUISystem::CFrontEndManager::GetInstance();
    ok = ok && first != 0 && g_ctorCalls == 1 && g_ctorThis == first && first->m_bQuitFromGame;
    ok = ok && g_newCalls == 2 && g_newSize == 0xE0;
    ok = ok && NUISystem::CFrontEndManager::GetInstance() == first && g_ctorCalls == 1 && g_newCalls == 2;
    if (ok) printf("FSE2_00595582_TEST PASS\n");
    else printf("FAIL ctor=%d new=%d size=%u\n", g_ctorCalls, g_newCalls, (unsigned)g_newSize);
    return ok ? 0 : 1;
}
