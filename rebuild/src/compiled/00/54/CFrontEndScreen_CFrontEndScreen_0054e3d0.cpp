// NUISystem::CFrontEndScreen::CFrontEndScreen @ 0x0054E3D0
// VC7.1, x86, /O2 /Oy.

extern "C" void __fastcall FableFrontEndScreenConstructBase(void*,void*,long);
extern "C" void* const FableFrontEndScreenVtable[];
extern "C" void* const FableFrontEndScreenInterface4[];
extern "C" void* const FableFrontEndScreenInterface18[];

extern "C" __declspec(naked) void* __fastcall
CFrontEndScreen_CFrontEndScreen_0054e3d0(void*,void*,long)
{
    __asm {
        mov eax,dword ptr [esp+4]
        push esi
        push eax
        mov esi,ecx
        call FableFrontEndScreenConstructBase
        xor eax,eax
        mov dword ptr [esi],offset FableFrontEndScreenVtable
        mov dword ptr [esi+4],offset FableFrontEndScreenInterface4
        mov dword ptr [esi+18h],offset FableFrontEndScreenInterface18
        mov dword ptr [esi+160h],eax
        mov dword ptr [esi+164h],eax
        mov dword ptr [esi+168h],eax
        mov eax,esi
        pop esi
        ret 4
    }
}
