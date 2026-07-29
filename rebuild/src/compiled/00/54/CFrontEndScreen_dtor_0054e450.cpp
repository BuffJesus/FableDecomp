// NUISystem::CFrontEndScreen::~CFrontEndScreen @ 0x0054E450
// VC7.1, x86, /O2 /Oy.

extern "C" void __cdecl FableFrontEndScreenFreeControl(void*);
extern "C" void __fastcall FableFrontEndScreenDestroyBase(void*,void*);
extern "C" void* const FableFrontEndScreenDestroyVtable[];
extern "C" void* const FableFrontEndScreenDestroyInterface4[];
extern "C" void* const FableFrontEndScreenDestroyInterface18[];

extern "C" __declspec(naked) void __fastcall
CFrontEndScreen_dtor_0054e450(void*,void*)
{
    __asm {
        push esi
        mov esi,ecx
        mov dword ptr [esi],offset FableFrontEndScreenDestroyVtable
        mov dword ptr [esi+4],offset FableFrontEndScreenDestroyInterface4
        mov dword ptr [esi+18h],offset FableFrontEndScreenDestroyInterface18
        mov eax,dword ptr [esi+164h]
        test eax,eax
        je clearMembers
        dec dword ptr [eax]
        mov eax,dword ptr [esi+164h]
        cmp dword ptr [eax],0
        jne clearMembers
        mov ecx,dword ptr [eax+8]
        call dword ptr [eax+4]
        mov eax,dword ptr [esi+164h]
        push eax
        call FableFrontEndScreenFreeControl
        add esp,4
clearMembers:
        mov dword ptr [esi+160h],0
        mov dword ptr [esi+164h],0
        mov ecx,esi
        pop esi
        jmp FableFrontEndScreenDestroyBase
    }
}
