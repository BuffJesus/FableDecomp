// Adjustor thunk for CAnimComponentFollowingOnAnim::TransferBinaryIn at 0x005dbe30.
// Retail bytes:  83 c1 08            add ecx, 8      ; this += 8 (base subobject adjust)
//                e9 c8 3d e7 ff      jmp 0x0044fc00  ; tail-call real impl (reloc)
// The +8 adjustment is a classic MSVC MI adjustor thunk; the jump target is an
// external symbol (reloc-masked), so this reaches RELOCATION_MATCH.
extern "C" void BaseTransferBinaryIn_005dbe30(void);

__declspec(naked) void TransferBinaryIn()
{
    __asm {
        add ecx, 8
        jmp BaseTransferBinaryIn_005dbe30
    }
}