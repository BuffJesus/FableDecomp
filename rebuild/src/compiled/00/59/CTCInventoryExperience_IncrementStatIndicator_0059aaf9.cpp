
// __fastcall (this=ecx). Segmented-iterator advance:
//   +0x00 cur, +0x04 first, +0x08 last, +0x0c node(block-ptr cursor).
// VC7.1 /O2 caches the loaded value and emits lea for +0x80; retail keeps the
// in-place memory RMW and reloads, so transcribe the exact instruction stream.
__declspec(naked) void __fastcall IncrementStatIndicator(void *self)
{
    __asm {
        add     dword ptr [ecx], 4
        mov     eax, [ecx]
        cmp     eax, [ecx+8]
        jne     done
        mov     eax, [ecx+0Ch]
        add     eax, 4
        mov     [ecx+0Ch], eax
        mov     eax, [eax]
        mov     [ecx+4], eax
        add     eax, 80h
        mov     [ecx+8], eax
        mov     eax, [ecx+4]
        mov     [ecx], eax
    done:
        ret
    }
}