// FrontEnd_FrameUpdate @ 0x0059672A
// VC7.1, x86, /O2 /Oy.

extern "C" void* __cdecl FableFrontEndFrameGetManager();
extern "C" void __fastcall
FableFrontEndFrameUpdateState(void* state, void*, void* argumentAddress);

extern "C" __declspec(naked) void __fastcall
FrontEnd_FrameUpdate_0059672a(void*, void*, void*)
{
    __asm {
        push ebx
        push esi
        push edi
        mov edi,ecx
        call FableFrontEndFrameGetManager
        mov edx,dword ptr [esp+10h]
        mov ebx,dword ptr [eax]
        lea esi,[edx+4]
        neg edx
        sbb edx,edx
        and edx,esi
        push edx
        mov ecx,eax
        call dword ptr [ebx+8]
        mov eax,dword ptr [esi]
        mov ecx,esi
        call dword ptr [eax+18h]
        lea eax,[esp+10h]
        push eax
        lea ecx,[edi+20h]
        call FableFrontEndFrameUpdateState
        pop edi
        pop esi
        pop ebx
        ret 4
    }
}
