// NUISystem::CFrontEndManager::Init @ 0x005958F5
// VC7.1, x86, /O2 /Oy.
//
// Initial boot resets the tail member, forwards the mesh-bank counted pointer
// to the base manager, publishes input/definition services, resets active
// screen state, establishes profile defaults, registers all known profiles,
// and destroys both incoming counted-pointer values.

extern "C" void __fastcall
FableFrontEndInitResetTail(void* tail, void*);
extern "C" void __fastcall
FableFrontEndInitCopyCountedPointer(
    void* destination,
    void*,
    const void* source);
extern "C" void* __cdecl
FableFrontEndInitGetBaseManager();
extern "C" void __fastcall
FableFrontEndInitSetGraphicsBank(
    void* manager,
    void*,
    unsigned long pointer,
    unsigned long counter);
extern "C" void __fastcall
FableFrontEndInitAddObserver(
    void* manager,
    void*,
    void* observer);
extern "C" void* __cdecl
FableFrontEndInitGetProfileService();
extern "C" void __fastcall
FableFrontEndInitSetProfileDefaults(void* service, void*);
extern "C" void __fastcall
FableFrontEndInitGetProfileNames(
    void* manager,
    void*,
    void* names);
extern "C" void __fastcall
FableFrontEndInitRegisterProfile(
    void* service,
    void*,
    void* profile);
extern "C" void __fastcall
FableFrontEndInitDestroyProfileNames(void* names, void*);
extern "C" void __fastcall
FableFrontEndInitDestroyGraphicsPointer(void* pointer, void*);
extern "C" void __fastcall
FableFrontEndInitDestroyMeshPointer(void* pointer, void*);

extern "C" __declspec(naked) void __fastcall
CFrontEndManager_Init_005958f5(
    void*,
    void*,
    unsigned long,
    unsigned long,
    unsigned long,
    unsigned long,
    void*,
    void*)
{
    __asm
    {
        push ebp
        mov ebp, esp
        sub esp, 0ch
        push esi
        mov esi, ecx
        push edi
        lea ecx, [esi + 0c4h]
        call FableFrontEndInitResetTail
        push ecx
        push ecx
        lea eax, [ebp + 10h]
        mov ecx, esp
        push eax
        call FableFrontEndInitCopyCountedPointer
        call FableFrontEndInitGetBaseManager
        mov ecx, eax
        call FableFrontEndInitSetGraphicsBank
        call FableFrontEndInitGetBaseManager
        mov ecx, dword ptr [ebp + 18h]
        push dword ptr [ebp + 1ch]
        mov dword ptr [eax + 20h], ecx
        call FableFrontEndInitGetBaseManager
        mov ecx, eax
        call FableFrontEndInitAddObserver
        xor edi, edi
        mov dword ptr [esi + 9ch], edi
        mov dword ptr [esi + 98h], edi
        call FableFrontEndInitGetProfileService
        mov ecx, eax
        call FableFrontEndInitSetProfileDefaults
        lea eax, [ebp - 0ch]
        push eax
        mov ecx, esi
        mov dword ptr [ebp - 0ch], edi
        mov dword ptr [ebp - 8], edi
        mov dword ptr [ebp - 4], edi
        call FableFrontEndInitGetProfileNames
        mov eax, dword ptr [ebp - 8]
        mov esi, dword ptr [ebp - 0ch]
        sub eax, esi
        sar eax, 2
        je profilesDone
        mov edi, eax

registerProfile:
        push esi
        call FableFrontEndInitGetProfileService
        mov ecx, eax
        call FableFrontEndInitRegisterProfile
        add esi, 4
        dec edi
        jne registerProfile

profilesDone:
        lea ecx, [ebp - 0ch]
        call FableFrontEndInitDestroyProfileNames
        lea ecx, [ebp + 8]
        call FableFrontEndInitDestroyGraphicsPointer
        lea ecx, [ebp + 10h]
        call FableFrontEndInitDestroyMeshPointer
        pop edi
        pop esi
        leave
        ret 18h
    }
}
