// NUISystem::CFrontEndManager::ChangeStateFirstTime @ 0x005952C3
// VC7.1, x86, /O2 /Oy.
//
// The first-time transition applies component state 5 to the current
// front-end stack entry.

extern "C" void** __fastcall
FableFrontEndFirstStateStackTop(void* stack, void*);

extern "C" __declspec(naked) void __fastcall
CFrontEndManager_ChangeStateFirstTime_005952c3(void*, void*)
{
    __asm
    {
        add ecx, 20h
        call FableFrontEndFirstStateStackTop
        mov ecx, dword ptr [eax]
        mov eax, dword ptr [ecx]
        push 5
        call dword ptr [eax + 0c0h]
        ret
    }
}
