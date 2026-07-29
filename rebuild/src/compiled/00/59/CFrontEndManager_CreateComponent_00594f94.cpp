// NUISystem::CFrontEndManager::CreateComponent @ 0x00594F94
// VC7.1, x86, /O2 /Oy.
//
// This overload forwards the requested definition name to the singleton
// manager's generic component factory with the front-end creation flag set.

extern "C" void* __cdecl
FableFrontEndCreateComponentGetManager();
extern "C" void* __fastcall
FableFrontEndCreateComponentFromDefinition(
    void* manager,
    void*,
    const void* definition,
    long frontEnd);

extern "C" __declspec(naked) void* __fastcall
CFrontEndManager_CreateComponent_00594f94(
    void*,
    void*,
    const void*)
{
    __asm
    {
        push 1
        push dword ptr [esp + 8]
        call FableFrontEndCreateComponentGetManager
        mov ecx, eax
        call FableFrontEndCreateComponentFromDefinition
        ret 4
    }
}
