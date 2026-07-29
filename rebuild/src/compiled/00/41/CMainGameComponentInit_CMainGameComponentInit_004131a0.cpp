// CMainGameComponentInit::CMainGameComponentInit @ 0x004131A0.
//
// The 16-byte transition record owns three wide strings at +0, +4, and +0xC,
// with one narrow string at +8.  Retail constructs them strictly in field
// order and returns the original object address.

extern "C" void* __fastcall
CMainGameComponentInit_WideCtor_004131a0(void* destination, void*);

extern "C" void* __fastcall
CMainGameComponentInit_NarrowCtor_004131a0(void* destination, void*);

extern "C" __declspec(naked) void* __fastcall
CMainGameComponentInit_CMainGameComponentInit_004131a0(void*, void*)
{
    __asm
    {
        push esi
        mov esi, ecx
        call CMainGameComponentInit_WideCtor_004131a0
        lea ecx, dword ptr [esi + 4]
        call CMainGameComponentInit_WideCtor_004131a0
        lea ecx, dword ptr [esi + 8]
        call CMainGameComponentInit_NarrowCtor_004131a0
        lea ecx, dword ptr [esi + 0Ch]
        call CMainGameComponentInit_WideCtor_004131a0
        mov eax, esi
        pop esi
        ret
    }
}
