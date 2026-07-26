#include <cstddef>
#include <cstdint>

class CScriptThing;

namespace
{
struct CScriptThing_MsgOpenedChest_DispatchTargetOverlay
{
    void** vftable; // +0x00
};
static_assert(offsetof(CScriptThing_MsgOpenedChest_DispatchTargetOverlay, vftable) == 0x00);

struct CScriptThing_MsgOpenedChest_SelfOverlay
{
    void** vftable; // +0x00
    CScriptThing_MsgOpenedChest_DispatchTargetOverlay* field04; // +0x04
};
static_assert(offsetof(CScriptThing_MsgOpenedChest_SelfOverlay, field04) == 0x04);
}

__declspec(naked) std::uint32_t __thiscall CScriptThing::MsgOpenedChest() const
{
    __asm
    {
        mov ecx, [ecx + 4]
        test ecx, ecx
        jne dispatch
        xor al, al
        ret

    dispatch:
        mov eax, [ecx]
        jmp dword ptr [eax + 0B4h]
    }
}