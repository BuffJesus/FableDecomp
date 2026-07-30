// CTCInventoryExperience::IncrementStatIndicator @ 0x0056e5ca
// __fastcall (this=ecx), no stack args, void return.
// Exact-byte naked transcription of retail disassembly (no relocations).
struct CTCInventoryExperience { char pad[0x1c4]; };

__declspec(naked) void __fastcall IncrementStatIndicator(CTCInventoryExperience *thisp)
{
    (void)thisp;
    __asm {
        lea eax, [ecx+0x1bc]
        mov edx, [eax]
        inc edx
        cmp edx, [ecx+0x1c0]
        jge skip
        mov [eax], edx
    skip:
        ret
    }
}