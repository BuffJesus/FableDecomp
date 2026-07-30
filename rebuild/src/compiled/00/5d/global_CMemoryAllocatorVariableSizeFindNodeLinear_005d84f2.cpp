
__declspec(naked) void FindNodeLinear()
{
    __asm {
        mov     eax, dword ptr [ecx]
        mov     ecx, dword ptr [eax+4]
        test    ecx, ecx
        je      done
        mov     edx, dword ptr [esp+4]
        mov     edx, dword ptr [edx]
    loop_top:
        cmp     dword ptr [ecx+0x10], edx
        jb      go_right
        mov     eax, ecx
        mov     ecx, dword ptr [ecx+8]
        jmp     test_cur
    go_right:
        mov     ecx, dword ptr [ecx+0xc]
    test_cur:
        test    ecx, ecx
        jne     loop_top
    done:
        ret     4
    }
}