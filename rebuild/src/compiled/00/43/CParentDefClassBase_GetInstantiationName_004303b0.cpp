// CParentDefClassBase::GetInstantiationName  @ 0x004303b0
// __thiscall (this in ECX), returns a CDefString (4-byte wrapper) by value
// via hidden return-struct pointer passed on the stack at [esp+4].
// Retail (12 bytes):
//   8b 49 25         mov ecx, [ecx+0x25]   ; load field this+0x25
//   8b 44 24 04      mov eax, [esp+4]      ; hidden return-struct ptr
//   89 08            mov [eax], ecx        ; store into return struct
//   c2 04 00         ret 4
__declspec(naked) void GetInstantiationName_004303b0()
{
    __asm {
        mov ecx, [ecx+0x25]
        mov eax, [esp+4]
        mov [eax], ecx
        ret 4
    }
}
