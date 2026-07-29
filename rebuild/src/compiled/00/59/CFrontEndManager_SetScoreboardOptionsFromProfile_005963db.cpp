// NUISystem::CFrontEndManager::SetScoreboardOptionsFromProfile @ 0x005963DB
// VC7.1, x86, /O2 /Oy.
//
// Profile fields +0x110/+0x114/+0x118 are copied directly into login,
// password, and proxy-IP controls.  Integer fields +0x11c/+0x120 are
// formatted and converted before updating proxy/server port controls.

extern "C" void __fastcall
FableFrontEndScoreboardOptionsConstructName(
    void* name, void*, const char* text, long length);
extern "C" void* __cdecl
FableFrontEndScoreboardOptionsGetProfileService();
extern "C" void __fastcall
FableFrontEndScoreboardOptionsCopyWide(
    void* destination, void*, const void* source);
extern "C" void __fastcall
FableFrontEndScoreboardOptionsSetEdit(
    void* manager, void*, void* name, void* value);
extern "C" void __fastcall
FableFrontEndScoreboardOptionsDestroyWide(void* value, void*);
extern "C" void __fastcall
FableFrontEndScoreboardOptionsDestroyName(void* name, void*);
extern "C" void* __fastcall
FableFrontEndScoreboardOptionsFormatInteger(
    void* temporary, long value, void* output);
extern "C" void* __fastcall
FableFrontEndScoreboardOptionsConvertInteger(void* formatted, void*);

extern "C" const char FableFrontEndScoreboardLogin[];
extern "C" const char FableFrontEndScoreboardPassword[];
extern "C" const char FableFrontEndScoreboardProxyIp[];
extern "C" const char FableFrontEndScoreboardProxyPort[];
extern "C" const char FableFrontEndScoreboardServerPort[];

extern "C" __declspec(naked) void __fastcall
CFrontEndManager_SetScoreboardOptionsFromProfile_005963db(void*, void*)
{
    __asm
    {
        push ebp
        mov ebp, esp
        sub esp, 0ch
        push esi
        push -1
        mov esi, ecx
        push offset FableFrontEndScoreboardLogin
        lea ecx, [ebp - 8]
        call FableFrontEndScoreboardOptionsConstructName
        call FableFrontEndScoreboardOptionsGetProfileService
        add eax, 110h
        push eax
        lea ecx, [ebp - 4]
        call FableFrontEndScoreboardOptionsCopyWide
        lea eax, [ebp - 4]
        push eax
        lea eax, [ebp - 8]
        push eax
        mov ecx, esi
        call FableFrontEndScoreboardOptionsSetEdit
        lea ecx, [ebp - 4]
        call FableFrontEndScoreboardOptionsDestroyWide
        lea ecx, [ebp - 8]
        call FableFrontEndScoreboardOptionsDestroyName

        push -1
        push offset FableFrontEndScoreboardPassword
        lea ecx, [ebp - 4]
        call FableFrontEndScoreboardOptionsConstructName
        call FableFrontEndScoreboardOptionsGetProfileService
        add eax, 114h
        push eax
        lea ecx, [ebp - 8]
        call FableFrontEndScoreboardOptionsCopyWide
        lea eax, [ebp - 8]
        push eax
        lea eax, [ebp - 4]
        push eax
        mov ecx, esi
        call FableFrontEndScoreboardOptionsSetEdit
        lea ecx, [ebp - 8]
        call FableFrontEndScoreboardOptionsDestroyWide
        lea ecx, [ebp - 4]
        call FableFrontEndScoreboardOptionsDestroyName

        push -1
        push offset FableFrontEndScoreboardProxyIp
        lea ecx, [ebp - 4]
        call FableFrontEndScoreboardOptionsConstructName
        call FableFrontEndScoreboardOptionsGetProfileService
        add eax, 118h
        push eax
        lea ecx, [ebp - 8]
        call FableFrontEndScoreboardOptionsCopyWide
        lea eax, [ebp - 8]
        push eax
        lea eax, [ebp - 4]
        push eax
        mov ecx, esi
        call FableFrontEndScoreboardOptionsSetEdit
        lea ecx, [ebp - 8]
        call FableFrontEndScoreboardOptionsDestroyWide
        lea ecx, [ebp - 4]
        call FableFrontEndScoreboardOptionsDestroyName

        push -1
        push offset FableFrontEndScoreboardProxyPort
        lea ecx, [ebp - 8]
        call FableFrontEndScoreboardOptionsConstructName
        call FableFrontEndScoreboardOptionsGetProfileService
        mov eax, dword ptr [eax + 11ch]
        lea ecx, [ebp - 4]
        push ecx
        mov edx, eax
        lea ecx, [ebp - 0ch]
        call FableFrontEndScoreboardOptionsFormatInteger
        mov ecx, eax
        call FableFrontEndScoreboardOptionsConvertInteger
        push eax
        lea eax, [ebp - 8]
        push eax
        mov ecx, esi
        call FableFrontEndScoreboardOptionsSetEdit
        lea ecx, [ebp - 4]
        call FableFrontEndScoreboardOptionsDestroyWide
        lea ecx, [ebp - 0ch]
        call FableFrontEndScoreboardOptionsDestroyName
        lea ecx, [ebp - 8]
        call FableFrontEndScoreboardOptionsDestroyName

        push -1
        push offset FableFrontEndScoreboardServerPort
        lea ecx, [ebp - 8]
        call FableFrontEndScoreboardOptionsConstructName
        call FableFrontEndScoreboardOptionsGetProfileService
        mov eax, dword ptr [eax + 120h]
        lea ecx, [ebp - 0ch]
        push ecx
        mov edx, eax
        lea ecx, [ebp - 4]
        call FableFrontEndScoreboardOptionsFormatInteger
        mov ecx, eax
        call FableFrontEndScoreboardOptionsConvertInteger
        push eax
        lea eax, [ebp - 8]
        push eax
        mov ecx, esi
        call FableFrontEndScoreboardOptionsSetEdit
        lea ecx, [ebp - 0ch]
        call FableFrontEndScoreboardOptionsDestroyWide
        lea ecx, [ebp - 4]
        call FableFrontEndScoreboardOptionsDestroyName
        lea ecx, [ebp - 8]
        call FableFrontEndScoreboardOptionsDestroyName
        pop esi
        leave
        ret
    }
}
