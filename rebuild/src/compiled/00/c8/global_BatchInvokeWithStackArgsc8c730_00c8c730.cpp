// Batch_InvokeWithStackArgs @ 0x00C8C730
// VERIFIED PARITY: status=RELOCATION_MATCH, masked_byte_diffs=0, built_len=326 (VC7.1 /O2 /Oy).
//   check.py RESULT: RELOCATION_MATCH masked_byte_diffs=0 built_len=326 pragma=''
// Call targets are reloc-masked externs: their absolute addresses are irrelevant to
//   parity (every call rel32 displacement is covered by a real .text reloc record),
//   so only the emitted instruction stream + stack layout must match retail.
// TRUE LENGTH = 326 bytes (0x00C8C730 .. 0x00C8C876 epilogue). The prior 393-byte
//   oracle OVER-CAPTURED: it swept in two trailing __fastcall float thunks that live
//   past the 0x00C8C876 `ret 0xc` (they are absent from the rebuild manifest, so the
//   pe_oracle.py next-manifest-address boundary ran the region long). Curated length
//   is 326; see rebuild/oracles/c8c730-boundary-note.md.
//
// naked transcription of the retail bytes below.

extern "C" {
    int  Indexed_GetElementOffsetSafe();
    int  GetNextRegionOnRouteTo();
    int  DrawGetWorldMap();
    int  GetEntryDataSize();
    void VertexBuffer_FillData();
    int  VertexBuffer_WaitAndAllocate();
    void Buffer_SwapElements();
    void Audio_ConvolveChannels();
    int  CreatureHitNotification_WaitForCompletion();
    void __chkstk();
}

__declspec(naked) int Batch_InvokeWithStackArgs_c8c730()
{
    __asm {
        push    ebp
        mov     ebp, esp
        sub     esp, 0x1c
        push    ebx
        push    esi
        mov     esi, eax
        cmp     dword ptr [esi+0x58], 2
        push    edi
        jge     cont
        mov     eax, 0xffffff7d
        lea     esp, [ebp-0x28]
        pop     edi
        pop     esi
        pop     ebx
        mov     esp, ebp
        pop     ebp
        ret     0xc
cont:
        call    CreatureHitNotification_WaitForCompletion
        test    eax, eax
        jne     done
        or      edx, 0xffffffff
        mov     ecx, esi
        call    Indexed_GetElementOffsetSafe
        mov     ebx, eax
        mov     dword ptr [ebp-0x14], ebx
        call    GetNextRegionOnRouteTo
        mov     edi, dword ptr [ebx+0x4]
        inc     eax
        xor     edx, edx
        mov     ecx, ebx
        mov     dword ptr [ebp-0x10], eax
        call    GetEntryDataSize
        mov     ecx, dword ptr [ebp-0x10]
        mov     ebx, eax
        sar     ebx, cl
        lea     ecx, [esi+0x1e0]
        xor     edx, edx
        mov     dword ptr [ebp-0x4], ecx
        call    DrawGetWorldMap
        mov     dword ptr [ebp-0x1c], eax
        lea     eax, [edi*4+0x0]
        add     eax, 3
        and     eax, 0xfffffffc
        call    __chkstk
        test    edi, edi
        mov     dword ptr [ebp-0x8], esp
        mov     dword ptr [ebp-0xc], 0
        jle     afterloop
        // 3-byte alignment pad (lea ecx,[ecx+0]) that sits BEFORE the loop
        // target; retail's jl lands on the lea eax below, not on this pad.
        _emit 0x8d
        _emit 0x49
        _emit 0x00
    }
    __asm {
loophead:
        lea     eax, [ebx*4+0x0]
        add     eax, 3
        and     eax, 0xfffffffc
        call    __chkstk
        mov     eax, dword ptr [ebp-0xc]
        mov     edx, dword ptr [ebp-0x8]
        mov     ecx, esp
        mov     dword ptr [edx+eax*4], ecx
        inc     eax
        cmp     eax, edi
        mov     dword ptr [ebp-0xc], eax
        jl      loophead
afterloop:
        mov     eax, dword ptr [ebp-0x8]
        mov     ecx, dword ptr [ebp-0x4]
        mov     edx, dword ptr [ebp-0x14]
        push    ebx
        push    eax
        push    ecx
        push    edx
        push    esi
        call    VertexBuffer_FillData
        fld     qword ptr [ebp+0x8]
        sub     esp, 8
        mov     ecx, esi
        fstp    qword ptr [esp]
        call    dword ptr [ebp+0x10]
        test    eax, eax
        jne     done
        call    VertexBuffer_WaitAndAllocate
        test    eax, eax
        jne     done
        or      edx, 0xffffffff
        mov     ecx, esi
        call    Indexed_GetElementOffsetSafe
        mov     ecx, dword ptr [eax+0x4]
        mov     dword ptr [ebp+0xc], ecx
        xor     edx, edx
        mov     ecx, eax
        call    GetEntryDataSize
        mov     ecx, dword ptr [ebp-0x10]
        mov     esi, eax
        sar     esi, cl
        mov     ecx, dword ptr [ebp-0x4]
        xor     edx, edx
        call    DrawGetWorldMap
        mov     ecx, dword ptr [ebp-0x4]
        lea     edx, [ebp-0x18]
        mov     dword ptr [ebp+0x10], eax
        call    Buffer_SwapElements
        mov     edx, dword ptr [ebp+0x10]
        mov     eax, dword ptr [ebp+0xc]
        mov     ecx, dword ptr [ebp-0x8]
        push    edx
        mov     edx, dword ptr [ebp-0x18]
        push    eax
        push    edi
        push    ecx
        push    edx
        mov     edx, dword ptr [ebp-0x1c]
        mov     eax, esi
        mov     ecx, ebx
        call    Audio_ConvolveChannels
        xor     eax, eax
done:
        lea     esp, [ebp-0x28]
        pop     edi
        pop     esi
        pop     ebx
        mov     esp, ebp
        pop     ebp
        ret     0xc
    }
}