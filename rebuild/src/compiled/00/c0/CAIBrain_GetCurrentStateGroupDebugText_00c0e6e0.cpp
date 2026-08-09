struct CCharString;

__declspec(naked) CCharString* __fastcall GetCurrentStateGroupDebugText(void* self)
{
    __asm {
        mov eax, dword ptr [ecx+4]
        test eax, eax
        jne L1
        mov eax, 1
        ret
    L1:
        mov ecx, dword ptr [ecx+4]
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax+8]
    }
}