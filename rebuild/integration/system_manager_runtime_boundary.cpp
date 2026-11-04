#include <new>
#include "fable_gfmain.h"
#include "fable_system_manager.h"
#include "test_support/bank_manager_member_stubs.h"
#include "test_support/profile_manager_member_stubs.h"
#include <string.h>

// Host recorders for CSystemManager::Initialise's subordinate engine services.
// The coordinator itself is reconstructed; these services remain boundaries.
extern "C" __declspec(dllimport) int FABLE_STDCALL VirtualProtect(void*, unsigned long, unsigned long, unsigned long*);
extern "C" char __ImageBase;
fable_u8 CSystemManager::NoInstances = 0;
#include "test_support/scratch_buffer_storage.h"
// Storage behind the retail CSystemManager singleton (0x013CA618): the exact
// Initialise probe runs on it, and GFGetSystemManager (0x009A4EC0) returns it.
extern "C" fable_u32 g_FableSystemManagerStorage[0x180 / 4] = {};
#pragma comment(linker, "/alternatename:?GSystemManager@@3VCSystemManager@@A=_g_FableSystemManagerStorage")
namespace {
    GFMainPhase9State* g_ActiveState;
    void** FindImportSlot(const char* name)
    {
        char* base = &__ImageBase;
        char* nt = base + *reinterpret_cast<fable_u32*>(base + 0x3C);
        const fable_u32 rva = *reinterpret_cast<fable_u32*>(nt + 0x80);
        if (rva == 0)
            return 0;
        for (fable_u32* entry = reinterpret_cast<fable_u32*>(base + rva);
             entry[3] != 0;
             entry += 5)
        {
            fable_u32* names = reinterpret_cast<fable_u32*>(
                base + (entry[0] ? entry[0] : entry[4]));
            void** slots = reinterpret_cast<void**>(base + entry[4]);
            for (int i = 0; names[i] != 0; ++i)
            {
                if ((names[i] & 0x80000000u) == 0 &&
                    strcmp(base + names[i] + 2, name) == 0)
                {
                    return &slots[i];
                }
            }
        }
        return 0;
    }

    bool SwapImport(const char* name, void* replacement, void** previous)
    {
        void** slot = FindImportSlot(name);
        unsigned long oldProtect = 0;
        if (slot == 0 ||
            !VirtualProtect(slot, sizeof(void*), 0x04, &oldProtect))
        {
            return false;
        }
        if (previous != 0)
            *previous = *slot;
        *slot = replacement;
        VirtualProtect(slot, sizeof(void*), oldProtect, &oldProtect);
        return true;
    }


    void* FABLE_STDCALL NoFiber(void*) { return 0; }
    long FABLE_STDCALL NoCOM(void*) { return 0; }
    void FABLE_STDCALL NoCriticalSection(void*) {}
    void* FABLE_STDCALL NoProcess() { return 0; }
}
extern "C" long (FABLE_STDCALL* FableSystemCoInitializeImport)(void*) = &NoCOM;
#pragma comment(linker, "/alternatename:__imp__CoInitialize@4=_FableSystemCoInitializeImport")

bool CInitBaseClass::Initialise() {
    if (g_ActiveState) ++g_ActiveState->exactSystemPreInitialiseCalls;
    return true;
}
void CSystemManager::InitGUIDFromName(const char*) {}
bool CSystemManager::InitWindow(long, long) {
    if (g_ActiveState) ++g_ActiveState->exactSystemWindowInitialiseCalls;
    return true;
}
bool FABLE_FASTCALL CSystemManager::DefaultErrorFunc(ESystemCode) { return false; }
void CSystemManager::InitSystemColours() { EverBeenActive = true; }
void CSystemManager::InitApplicationName() {}
bool CScratchBuffer::Initialise(long) { return true; }
void FABLE_FASTCALL GFInitMaths() {}
void NVectorMath::GFInitVectorMath() {}

template<typename T> void CCountedPointer<T>::Reset(T* data) { Data = data; }
template void CCountedPointer<CDisplayManager>::Reset(CDisplayManager*);
template void CCountedPointer<CInputManager>::Reset(CInputManager*);
template void CCountedPointer<CDriveManager>::Reset(CDriveManager*);
template void CCountedPointer<CASoundManager>::Reset(CASoundManager*);
template void CCountedPointer<CFontManager>::Reset(CFontManager*);
template void CCountedPointer<CGraphicBankManager>::Reset(CGraphicBankManager*);
template void CCountedPointer<CMeshBankManager>::Reset(CMeshBankManager*);
template void CCountedPointer<CProfileManager>::Reset(CProfileManager*);
template<typename T> CCountedPointer<T> CTBaseSingletonNoDefaultAlloc<T>::PClass;
CDriveManager::~CDriveManager() {}
ESystemCode CDriveManager::Initialise(CSystemManager*) {
    if (g_ActiveState) ++g_ActiveState->exactSystemDriveInitialiseCalls;
    return kSystemCodeSuccess;
}
// Authored display boundary; native device-creation defaults remain unrecovered.

CDisplayManager::CDisplayManager(CSystemManager&) {
    if (g_ActiveState) ++g_ActiveState->exactSystemDisplayConstructCalls;
}
void CDisplayManager::Init(const CDisplayManagerInit&, bool) {}
void CDisplayManager::AddDeviceResetCallback(CDeviceResetCallback*) {
    if (g_ActiveState) ++g_ActiveState->exactSystemComponentAddCalls;
}
// The coordinator probe owns empty input containers, not a live input service.
CInputManager::CInputManager(const CInputManagerInit&, bool) : Checksum(0, 0, 0) {}
CInputManager::~CInputManager() {}
CInputEvent::~CInputEvent() { abort(); }
CInputManagerDX::CInputManagerDX(const CInputManagerInit& init, bool exclusiveMode)
    : CInputManager(init, exclusiveMode) {
    if (g_ActiveState) ++g_ActiveState->exactSystemInputConstructCalls;
}
CSoundManagerLH::CSoundManagerLH(const CSoundManagerInit&, bool) {}
CFontManager::CFontManager(const CCharString&, const CCharString&) {}
CFontManager::~CFontManager() {}
CGraphicBankManager::CGraphicBankManager() {
    if (g_ActiveState) ++g_ActiveState->exactSystemTextConstructCalls;
}
CMeshBankManager::CMeshBankManager() {
    if (g_ActiveState) ++g_ActiveState->exactSystemMeshConstructCalls;
}
CProfileManager::CProfileManager() {
    if (g_ActiveState) ++g_ActiveState->exactSystemColoursConstructCalls;
}
void CDeviceResetCallback::OnPreDeviceReset() {}
bool CDeviceResetCallback::OnPostDeviceReset() { return true; }
void CMeshBankManager::OnPreDeviceReset() {}
bool CMeshBankManager::OnPostDeviceReset() { return true; }
void CASoundManager::UnknownSlot01() {}
void CASoundManager::UnknownSlot02() {}
void CASoundManager::UnknownSlot03() {}
void CASoundManager::UnknownSlot04() {}
void CASoundManager::UnknownSlot05() {}
void CASoundManager::UnknownSlot06() {}
void CASoundManager::UnknownSlot07() {}
void CASoundManager::UnknownSlot08() {}
void CASoundManager::UnknownSlot09() {}
void CASoundManager::UnknownSlot10() {}
void CASoundManager::UnknownSlot11() {}
void CASoundManager::UnknownSlot12() {}
void CASoundManager::UnknownSlot13() {}
void CASoundManager::UnknownSlot14() {}
void CASoundManager::UnknownSlot15() {}
void CASoundManager::UnknownSlot16() {}
void CASoundManager::UnknownSlot17() {}
void CASoundManager::UnknownSlot18(long) {}
bool CInputManager::SetExclusiveMode(bool) { return false; }
void CInputManager::Update() {}
void CASoundManager::UnknownSlot19() {}
void CASoundManager::UnknownSlot20() {}
void CASoundManager::UnknownSlot21() {}
void CASoundManager::UnknownSlot22() {}
void CASoundManager::UnknownSlot23() {}
void CASoundManager::UnknownSlot24() {}
void CASoundManager::UnknownSlot25() {}
void CASoundManager::UnknownSlot26() {}
void CASoundManager::UnknownSlot27() {}
void CASoundManager::UnknownSlot28() {}
void CASoundManager::UnknownSlot29() {}
void CASoundManager::UnknownSlot30() {}
void CASoundManager::UnknownSlot31() {}
void CASoundManager::UnknownSlot32() {}
void CASoundManager::UnknownSlot33() {}
void CASoundManager::UnknownSlot34() {}
void CASoundManager::UnknownSlot35() {}
void CASoundManager::UnknownSlot36() {}
void CASoundManager::UnknownSlot37() {}
void CASoundManager::UnknownSlot38() {}
void CASoundManager::UnknownSlot39() {}
void CASoundManager::UnknownSlot40() {}
void CASoundManager::UnknownSlot41() {}
void CASoundManager::UnknownSlot42() {}
void CASoundManager::UnknownSlot43() {}
void CASoundManager::UnknownSlot44() {}
void CASoundManager::UnknownSlot45() {}
void CASoundManager::UnknownSlot46(unsigned long, float, float) {}
float CASoundManager::UnknownSlot47(unsigned long) { return 0.0f; }
void CASoundManager::UnknownSlot48() {}
bool CASoundManager::UnknownSlot49(float&) { return false; }
void CASoundManager::UnknownSlot50() {}
void CASoundManager::GenerateLipSyncData(const CWideString&, LHA::CBinaryDataWriter&) {}

fable_i32 FABLE_FASTCALL FableRunExactSystemManagerInitialiseBoundary(
    CSystemManagerInit* init, GFMainPhase9State& state)
{
    memset(g_FableSystemManagerStorage, 0, sizeof(g_FableSystemManagerStorage));
    CSystemManager::NoInstances = 0;
    void* fiber = 0;
    void* critical = 0;
    void* process = 0;
    const bool redirected =
        SwapImport("ConvertThreadToFiber", reinterpret_cast<void*>(&NoFiber), &fiber) &&
        SwapImport("InitializeCriticalSection", reinterpret_cast<void*>(&NoCriticalSection), &critical) &&
        SwapImport("GetCurrentProcess", reinterpret_cast<void*>(&NoProcess), &process);
    g_ActiveState = &state;
    CSystemManager* manager = reinterpret_cast<CSystemManager*>(g_FableSystemManagerStorage);
    const ESystemCode result = redirected ? manager->Initialise(init) : static_cast<ESystemCode>(0);
    g_ActiveState = 0;
    if (process) SwapImport("GetCurrentProcess", process, 0);
    if (critical) SwapImport("InitializeCriticalSection", critical, 0);
    if (fiber) SwapImport("ConvertThreadToFiber", fiber, 0);
    // operator new now belongs to the CRT, not the old counted arena.
    state.exactSystemAllocationCalls = 0;
    state.observedExactSystemInitialiseCount = CSystemManager::NoInstances;
    // These boundary-only objects are not installed into the live engine.
    // Release the CRT allocations before a later probe reuses the storage.
    delete manager->PInputManager.Data;
    delete manager->PDisplayManager.Data;
    delete manager->PGraphicBankManager.Data;
    delete manager->PMeshBankManager.Data;
    delete manager->PSoundManager.Data;
    delete manager->PFontManager.Data;
    delete manager->ProfileManager.Data;
    if (redirected) {
        delete &CDriveManager::Get();
        CDriveManager::Set(0);
    }
    manager->WinAppName.~CWideString();
    manager->TempPath.~CWideString();
    manager->ApplicationName.~CCharString();
    return result;
}

#include "test_support/display_ownership_boundary.h"
