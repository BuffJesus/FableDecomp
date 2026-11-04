#include "fable_game.h"
#include "fable_nui_manager.h"
#include "fable_system_manager.h"
#include <stdio.h>
#include <string.h>

CGraphicBankManager* g_FableGraphicBankManager_013B837C;

static int seq;
static bool ok;
static unsigned char* frontend;
static int dtorAt, baseDtorAt, engineAt, gfxCopyAt, gfxCloseAt, gfxRelAt,
    sysAt, meshCopyAt, meshCloseAt, meshRelAt, fontRelAt;
static void* closedGfx;
static void* closedMesh;

// CFrontEndManager::MediaPlayerErrorString is a CWideString member.
CWideString::CWideString() {}
CWideString::~CWideString() {}
NUISystem::CObserver::CObserver() {}
CCharString::~CCharString() {}
namespace _STL {
template<class T,class A> list<T,A>::~list(){free(_M_node);}
template<class T,class A> _Deque_base<T,A>::_Deque_base(const A&,size_t){}
template<class T,class A> deque<T,A>::deque():_Deque_base<T,A>(A(),0){}
}
NUISystem::CObserver::~CObserver() { baseDtorAt = ++seq; }
// fable_nui_manager.h declares retail's out-of-line ctor (0x005953E2);
// the fixture's stand-in only has to exist.
NUISystem::CFrontEndManager::CFrontEndManager() {}
NUISystem::CFrontEndManager::~CFrontEndManager()
{
    dtorAt = ++seq;
    ok = ok && static_cast<void*>(this) == *reinterpret_cast<void**>(frontend + 0xB4);
}

static unsigned long gfxManager[2];
static unsigned long meshManager[3];
static unsigned long systemManager[0x180 / 4];

CSystemManager* FABLE_FASTCALL GFGetSystemManager()
{
    sysAt = ++seq;
    return reinterpret_cast<CSystemManager*>(systemManager);
}

template <> CCountedPointer<CGraphicDataBank>::CCountedPointer(const CCountedPointer& other)
    : Data(other.Data), Info(other.Info)
{
    gfxCopyAt = ++seq;
    ok = ok && &other == reinterpret_cast<const CCountedPointer*>(frontend + 0x40);
}
template <> CCountedPointer<CMeshDataBank>::CCountedPointer(const CCountedPointer& other)
    : Data(other.Data), Info(other.Info)
{
    meshCopyAt = ++seq;
    ok = ok && &other == reinterpret_cast<const CCountedPointer*>(frontend + 0x48);
}
void CGraphicBankManager::CloseGraphicBank(CCountedPointer<CGraphicDataBank> bank)
{
    gfxCloseAt = ++seq;
    closedGfx = bank.Data;
    ok = ok && static_cast<void*>(this) == gfxManager;
}
void CMeshBankManager::CloseMeshBank(CCountedPointer<CMeshDataBank> bank)
{
    meshCloseAt = ++seq;
    closedMesh = bank.Data;
    ok = ok && static_cast<void*>(this) == meshManager;
}
// ReleaseData stand-ins clear both fields like retail (0x0042F974 etc.) but
// scribble Data afterwards so the caller's own Data store is observable.
template <> void CCountedPointer<CIEngine>::ReleaseData()
{
    engineAt = ++seq;
    ok = ok && static_cast<void*>(this) == frontend + 0x58;
    Info = 0;
    Data = reinterpret_cast<CIEngine*>(0x77);
}
template <> void CCountedPointer<CGraphicDataBank>::ReleaseData()
{
    gfxRelAt = ++seq;
    ok = ok && static_cast<void*>(this) == frontend + 0x40;
    Info = 0;
    Data = reinterpret_cast<CGraphicDataBank*>(0x77);
}
template <> void CCountedPointer<CMeshDataBank>::ReleaseData()
{
    meshRelAt = ++seq;
    ok = ok && static_cast<void*>(this) == frontend + 0x48;
    Info = 0;
    Data = reinterpret_cast<CMeshDataBank*>(0x77);
}
template <> void CCountedPointer<const CFontBank>::ReleaseData()
{
    fontRelAt = ++seq;
    ok = ok && static_cast<void*>(this) == frontend + 0x10;
    Info = 0;
    Data = reinterpret_cast<const CFontBank*>(0x77);
}

static void Put(unsigned char* b, int off, unsigned long v) { memcpy(b + off, &v, 4); }
static unsigned long Get(unsigned char* b, int off) { unsigned long v; memcpy(&v, b + off, 4); return v; }

static bool Run(bool mgr, bool engine, bool gfx, bool mesh)
{
    unsigned long storage[sizeof(CNewFrontendGameComponent) / 4];
    unsigned char expected[sizeof(storage)];
    memset(storage, 0xA5, sizeof(storage));
    frontend = reinterpret_cast<unsigned char*>(storage);
    NUISystem::CFrontEndManager* m = mgr ? new NUISystem::CFrontEndManager : 0;
    Put(frontend, 0xB4, (unsigned long)m);
    Put(frontend, 0x58, engine ? 0x5800 : 0);
    Put(frontend, 0x40, gfx ? 0x4000 : 0);
    Put(frontend, 0x48, mesh ? 0x4800 : 0);
    // The by-value bank copies are destroyed inside Close*Bank (the header's
    // ~CCountedPointer); give them no pointer-info block to drop.
    Put(frontend, 0x44, 0);
    Put(frontend, 0x4C, 0);
    Put(frontend, 0x10, 0x1000);
    memcpy(expected, storage, sizeof(storage));
    if (mgr) Put(expected, 0xB4, 0);
    if (engine) { Put(expected, 0x58, 0); Put(expected, 0x5C, 0); }
    if (gfx) { Put(expected, 0x40, 0); Put(expected, 0x44, 0); }
    if (mesh) { Put(expected, 0x48, 0); Put(expected, 0x4C, 0); }
    Put(expected, 0x10, 0); Put(expected, 0x14, 0);

    g_FableGraphicBankManager_013B837C = reinterpret_cast<CGraphicBankManager*>(gfxManager);
    memset(systemManager, 0, sizeof(systemManager));
    systemManager[0x74 / 4] = (unsigned long)meshManager;
    seq = dtorAt = baseDtorAt = engineAt = gfxCopyAt = gfxCloseAt = gfxRelAt = sysAt =
        meshCopyAt = meshCloseAt = meshRelAt = fontRelAt = 0;
    closedGfx = closedMesh = 0;
    ok = true;

    reinterpret_cast<CNewFrontendGameComponent*>(storage)->CNewFrontendGameComponent::CleanUpInterfaces();

    int n = 1;
    ok = ok && (mgr ? (dtorAt == n++ && baseDtorAt == n++) : (dtorAt == 0 && baseDtorAt == 0));
    ok = ok && (engine ? engineAt == n++ : engineAt == 0);
    if (gfx)
        ok = ok && gfxCopyAt == n++ && gfxCloseAt == n++ && gfxRelAt == n++ &&
            closedGfx == (void*)0x4000;
    else
        ok = ok && gfxCopyAt == 0 && gfxCloseAt == 0 && gfxRelAt == 0;
    if (mesh)
        ok = ok && sysAt == n++ && meshCopyAt == n++ && meshCloseAt == n++ &&
            meshRelAt == n++ && closedMesh == (void*)0x4800;
    else
        ok = ok && sysAt == 0 && meshCopyAt == 0 && meshCloseAt == 0 && meshRelAt == 0;
    ok = ok && fontRelAt == n++;
    ok = ok && memcmp(expected, storage, sizeof(storage)) == 0;
    return ok;
}

int main()
{
    if (!Run(true, true, true, true)) { puts("case1"); return 1; }
    if (!Run(false, false, false, false)) { puts("case2"); return 1; }
    if (!Run(true, false, true, false)) { puts("case3"); return 1; }
    if (!Run(false, true, false, true)) { puts("case4"); return 1; }
    puts("FABLETLC_NEW_FRONTEND_CLEANUP_INTERFACES PASS");
    return 0;
}