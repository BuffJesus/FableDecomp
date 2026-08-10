extern "C" int __fastcall sub_a01b50(void* self);

// Over-capture: IsTaskAvailable (14 bytes) + adjacent tail-forwarder (8 bytes).
// add ecx,0x10 ; call sub_a01b50 ; neg;sbb;inc ; ret ; add ecx,8 ; jmp sub_a01b50
__declspec(naked) bool __fastcall IsTaskAvailable(void* /*self*/)
{
    __asm {
        add ecx, 0x10
        call sub_a01b50
        neg eax
        sbb eax, eax
        inc eax
        ret
        add ecx, 8
        jmp sub_a01b50
    }
}