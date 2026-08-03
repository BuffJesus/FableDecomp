// Retail 0x0042ad6c: std::_Tree lower_bound (find first node with key >= *keyptr).
// this=ecx (tree); single stack arg = pointer to key; ret 4.
// head at [ecx]; root at [head+4]; node key at +0x10; +0xc=greater child,
// +0x8=lesser-or-equal child; result = last node where node->key >= key
// (head sentinel if none). __declspec(naked) exact transcription.
__declspec(naked) void BinarySearchTree_FindEqualOrGreater(){
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
        jmp     test_node
    go_right:
        mov     ecx, dword ptr [ecx+0xc]
    test_node:
        test    ecx, ecx
        jne     loop_top
    done:
        ret     4
    }
}