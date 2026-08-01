// COpinionTransientOffset::TransferBinaryIn @ 0x004793f8 (81 bytes).
//
// __fastcall: this (ecx) = destination buffer (0x18 bytes = 6 dwords);
// one stack arg [esp+4] = CDataInputStream* stream.
//
// Reads 0x18 bytes out of the stream's in-memory source window when the whole
// record fits contiguously (the classic MSVC inlined fast path with the 64-bit
// signed <= 0x7FFFFFFF overflow guard), otherwise tail-calls the general
// windowed reader.  Stream layout (offsets from the disasm):
//   +0x04 : long  cursor/position       (bumped by 0x18 on the fast path)
//   +0x0C : void* source read pointer    (bumped by 0x18)
//   +0x14 : long  bytes left in window   (decremented by 0x18)
// The slow-path callee (CDataInputStream::ReadWithSrcChunkOverflow-style helper)
// is reloc-masked, so it is declared extern.

extern "C" void __fastcall
OpinionTransferBinaryIn_slowread_0051a8a8(void*, void*, void*, long);

extern "C" __declspec(naked) void __fastcall
COpinionTransientOffset_TransferBinaryIn_004793f8(void* /*ecx this*/, void* /*edx unused*/, void* /*stream*/)
{
    __asm
    {
        mov     eax, dword ptr [esp + 4]
        push    edi
        mov     edi, ecx
        mov     ecx, dword ptr [eax + 4]
        add     ecx, 18h
        xor     edx, edx
        jl      done
        jg      high_valid
        test    ecx, ecx
        jb      done

    high_valid:
        test    edx, edx
        jg      done
        jl      window_check
        cmp     ecx, 7FFFFFFFh
        ja      done

    window_check:
        cmp     dword ptr [eax + 14h], 18h
        jl      slow_path

        push    esi
        mov     esi, dword ptr [eax + 0Ch]
        push    6
        pop     ecx
        rep movsd
        add     dword ptr [eax + 0Ch], 18h
        add     dword ptr [eax + 14h], -18h
        add     dword ptr [eax + 4], 18h
        pop     esi
        jmp     done

    slow_path:
        push    18h
        push    edi
        mov     ecx, eax
        call    OpinionTransferBinaryIn_slowread_0051a8a8

    done:
        pop     edi
        ret     4
    }
}