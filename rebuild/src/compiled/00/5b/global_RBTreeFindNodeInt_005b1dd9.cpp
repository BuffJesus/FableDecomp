__declspec(naked) void* RBTree_FindNode_Int(void* keyPtr)
{
    (void)keyPtr;
    __asm {
        mov eax, dword ptr [ecx]
        mov ecx, dword ptr [eax+4]
        test ecx, ecx
        je done
        mov edx, dword ptr [esp+4]
        mov edx, dword ptr [edx]
    loop_top:
        cmp dword ptr [ecx+0x10], edx
        jl go_left
        mov eax, ecx
        mov ecx, dword ptr [ecx+8]
        jmp after
    go_left:
        mov ecx, dword ptr [ecx+0xc]
    after:
        test ecx, ecx
        jne loop_top
    done:
        ret 4
    }
}