#include <cstddef>
#include <cstdint>

struct CScriptThing_MsgReceivedMoney_Overlay
{
    std::uint8_t pad_0[0x4];
    void* object_at_4;
};
static_assert(offsetof(CScriptThing_MsgReceivedMoney_Overlay, object_at_4) == 0x4);

__declspec(naked) std::uint32_t CScriptThing::MsgReceivedMoney(long& a1) const
{
    __asm {
        mov ecx, [ecx + 0x4]
        test ecx, ecx
        jne has_target
        xor al, al
        ret 4

    has_target:
        mov eax, [ecx]
        jmp dword ptr [eax + 0x88]
    }
}