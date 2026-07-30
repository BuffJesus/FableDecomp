extern "C" void SubTarget();
__declspec(naked) void __fastcall DetachTarget(void)
{
    __asm {
        mov  edx, dword ptr [ecx+0x24]
        push esi
        mov  esi, dword ptr [ecx+0x8]
        lea  ecx, [edx+edx*4]
        xor  eax, eax
        shl  ecx, 2
        cmp  dword ptr [ecx+esi*1+0x4], eax
        je   done
        add  ecx, esi
        pop  esi
        jmp  SubTarget
    done:
        pop  esi
        ret
    }
}