// CTCTrapBase::ManualReset @ 0x004ae9a0
// Naked transcription of retail disassembly:
//   mov al,[ecx+0x2662]; test al,al; je ret; add ecx,0x2010; jmp target; ret 4
extern "C" void FableTLC_ManualReset_Target(void);  // reloc-masked extern (retail 0x9f1650)

__declspec(naked) void ManualReset(int /*arg*/)
{
    __asm {
        mov     al, byte ptr [ecx+0x2662]
        test    al, al
        je      done
        add     ecx, 0x2010
        jmp     FableTLC_ManualReset_Target
    done:
        ret     4
    }
}