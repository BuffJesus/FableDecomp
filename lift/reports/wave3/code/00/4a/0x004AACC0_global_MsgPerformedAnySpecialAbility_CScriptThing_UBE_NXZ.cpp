#include <cstddef>
#include <cstdint>

class CScriptThing;

namespace
{
struct CScriptThing_MsgPerformedAnySpecialAbility_DispatchTargetOverlay
{
    void** vftable; // +0x00
};
static_assert(offsetof(CScriptThing_MsgPerformedAnySpecialAbility_DispatchTargetOverlay, vftable) == 0x00);

struct CScriptThing_MsgPerformedAnySpecialAbility_SelfOverlay
{
    void** vftable; // +0x00
    CScriptThing_MsgPerformedAnySpecialAbility_DispatchTargetOverlay* field04; // +0x04
};
static_assert(offsetof(CScriptThing_MsgPerformedAnySpecialAbility_SelfOverlay, field04) == 0x04);

static_assert(0x98 % sizeof(void*) == 0);
}

__declspec(naked) bool __thiscall CScriptThing::MsgPerformedAnySpecialAbility() const
{
    __asm
    {
        mov ecx, [ecx + 0x04]
        test ecx, ecx
        jne dispatch
        xor al, al
        ret

    dispatch:
        mov eax, [ecx]
        jmp dword ptr [eax + 0x98]
    }
}