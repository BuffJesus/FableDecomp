// CMainGameComponentInit::CMainGameComponentInit(copy) @ 0x00415E17.
//
// The transition record is copied in field order: wide +0, wide +4,
// narrow +8, wide +0xC.  Each string copy constructor consumes one source
// pointer from the stack and the constructor returns the destination record.

extern "C" void* __fastcall
CMainGameComponentInit_WideCopyCtor_00415e17(
    void* destination,
    void*,
    const void* source);

extern "C" void* __fastcall
CMainGameComponentInit_NarrowCopyCtor_00415e17(
    void* destination,
    void*,
    const void* source);

extern "C" __declspec(naked) void* __fastcall
CMainGameComponentInit_CMainGameComponentInit_00415e17(
    void*,
    void*,
    const void*)
{
    __asm
    {
        push esi
        push edi
        mov edi, dword ptr [esp + 0Ch]
        push edi
        mov esi, ecx
        call CMainGameComponentInit_WideCopyCtor_00415e17
        lea eax, dword ptr [edi + 4]
        push eax
        lea ecx, dword ptr [esi + 4]
        call CMainGameComponentInit_WideCopyCtor_00415e17
        lea eax, dword ptr [edi + 8]
        push eax
        lea ecx, dword ptr [esi + 8]
        call CMainGameComponentInit_NarrowCopyCtor_00415e17
        add edi, 0Ch
        push edi
        lea ecx, dword ptr [esi + 0Ch]
        call CMainGameComponentInit_WideCopyCtor_00415e17
        pop edi
        mov eax, esi
        pop esi
        ret 4
    }
}
