// UInt_RBTreeFindLess @ 0x0057fed6
// __fastcall (this=ecx), one stack arg = pointer to key, ret 4.
// Naked transcription of retail bytes for exact parity.
__declspec(naked) void UInt_RBTreeFindLess()
{
    __asm {
        mov  eax, dword ptr [ecx]
        mov  ecx, dword ptr [eax+0x4]
        test ecx, ecx
        je   done
        mov  edx, dword ptr [esp+0x4]
        mov  edx, dword ptr [edx]
    loop_top:
        cmp  edx, dword ptr [ecx+0x10]
        jae  go_right
        mov  eax, ecx
        mov  ecx, dword ptr [ecx+0x8]
        jmp  test_node
    go_right:
        mov  ecx, dword ptr [ecx+0xc]
    test_node:
        test ecx, ecx
        jne  loop_top
    done:
        ret  0x4
    }
}