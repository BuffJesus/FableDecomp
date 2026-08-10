extern float g_dwConst;
__declspec(naked) float __fastcall GetDamageMultiplier(void* self) {
    __asm {
        mov eax, dword ptr [ecx+0x10]
        mov ecx, dword ptr [ecx+0x44]
        mov edx, dword ptr [eax+0x128]
        fld dword ptr [edx+ecx*4]
        fmul dword ptr [g_dwConst]
        ret
    }
}