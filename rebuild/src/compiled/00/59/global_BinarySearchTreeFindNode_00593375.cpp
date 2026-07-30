// BinarySearchTree_FindNode @ 0x00593375 (__fastcall, ret 4)
// STL-style tree lower_bound: [ecx]=header, [eax+4]=root, node key at +0x10,
// left child +0x8, right child +0xc. Walks tree remembering last node whose
// key >= *pkey. Exact-byte naked transcription of retail.
__declspec(naked) void BinarySearchTree_FindNode()
{
    __asm {
        mov  eax, [ecx]        // eax = header
        mov  ecx, [eax+4]      // ecx = root
        test ecx, ecx
        je   L_done
        mov  edx, [esp+4]      // edx = pkey
        mov  edx, [edx]        // edx = *pkey (search key)
    L_cmp:
        cmp  [ecx+0x10], edx   // node->key vs search key
        jl   L_right           // node->key < key -> go right
        mov  eax, ecx          // remember node
        mov  ecx, [ecx+8]      // go left
        jmp  L_test
    L_right:
        mov  ecx, [ecx+0xc]    // go right
    L_test:
        test ecx, ecx
        jne  L_cmp
    L_done:
        ret  4
    }
}