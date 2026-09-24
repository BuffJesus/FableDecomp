#pragma once

// Retail singleton slot 013B8710. Constructor 0041E3F6 and operator new
// 00BFEA1A remain explicit integration boundaries.
// RTTI identifies NUISystem::CManager; the historical FrontEnd link names
// below remain compatibility symbols, not a claim about the concrete owner.
extern "C" void* FableFrontEndManagerInstance;
extern "C" void* FableFrontEndManagerAllocate(unsigned long);
extern "C" void* __fastcall FableFrontEndManagerConstruct(void*);
extern "C" void* CFrontEndManager_GetInstance_0041e5f2();
void* __cdecl FableUiGetManager();
