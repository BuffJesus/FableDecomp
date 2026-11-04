#pragma once
// Authored startup-test boundary. These lifetimes support the mock display
// constructor; they are not providers of recovered display/busy-manager code.
#include "fable_display_viewport.h"
#include "fable_stlport_uninitialized.h"
namespace _STL {
template<> list<CDeviceResetCallback*>::~list() { free(_M_node); }
template<> list<CBusyResourceManager::CPendingQuery>::~list() { free(_M_node); }
}
CBusyResourceManager::CBusyResourceManager()
    : PendingFrame(1), FinishedFrame(0), D3DDevice(0) {}
CBusyResourceManager::~CBusyResourceManager() {}
CDisplayManager::~CDisplayManager() {}
// Exact native resource implementations take precedence when the integration
// executable links them. Otherwise the empty mock resources need no action.
extern "C" void FABLE_FASTCALL FableDisplayEmptyResourceBoundary(void*) {}
#pragma comment(linker,"/alternatename:?Uninitialise@CSurface@@QAEXXZ=@FableDisplayEmptyResourceBoundary@4")
#pragma comment(linker,"/alternatename:?Uninitialise@CTexture@@QAEXXZ=@FableDisplayEmptyResourceBoundary@4")

#pragma comment(linker, "/alternatename:??1CVsyncCallbackTimer@@QAE@XZ=@FableDisplayEmptyResourceBoundary@4")
