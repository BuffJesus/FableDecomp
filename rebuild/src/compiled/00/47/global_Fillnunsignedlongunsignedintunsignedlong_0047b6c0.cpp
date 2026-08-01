/* _Fill_n<unsigned_long*,unsigned_int,unsigned_long> @ retail 0x0047b6c0
 *
 * MSVC 2003 / VC7.1, __fastcall (ecx=destination, edx=count, [esp+4]=value ptr).
 * Fills `count` copies of a 12-byte value (three 4-byte sub-objects at +0/+4/+8)
 * into the destination, copy-constructing each sub-object via the masked
 * __fastcall helper at 0x520060 (this=ecx=&dest.sub, one stack arg=&src.sub).
 * Returns the advanced destination pointer in eax.
 *
 * Naked-asm reconstruction mirroring the landed sibling
 * std_Fill_n_ulong_0066ec31; the call rel32 operand is relocation-masked in parity.
 */

extern "C" void Sub_520060(void);   /* masked __fastcall subobject copy helper */

extern "C" __declspec(naked) void __fastcall
_Fill_n_ulong_ptr_uint_ulong_0047b6c0(
    void* destination, unsigned int count, const void* value)
{
    __asm
    {
        test edx, edx
        push esi
        mov  esi, ecx
        jbe  done

        push ebx
        mov  ebx, [esp + 0Ch]
        push ebp
        push edi
        mov  ebp, edx

    loop_top:
        test esi, esi
        je   skip

        push ebx
        mov  ecx, esi
        call Sub_520060
        lea  eax, [ebx + 4]
        lea  edi, [esi + 8]
        push eax
        lea  ecx, [edi - 4]
        call Sub_520060
        lea  ecx, [ebx + 8]
        push ecx
        mov  ecx, edi
        call Sub_520060

    skip:
        add  esi, 0Ch
        dec  ebp
        jne  loop_top

        pop  edi
        pop  ebp
        pop  ebx

    done:
        mov  eax, esi
        pop  esi
        ret  8
    }
}