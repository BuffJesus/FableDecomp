#include <cstddef>

class CScriptThing
{
public:
    long __thiscall MsgHowLongWasExpressionPerformed() const;
};

namespace
{
struct CScriptThing_MsgHowLongWasExpressionPerformed_ThisOverlay
{
    std::byte m_Pad00[0x04];
    void* m_Field04; // 0x04
};
static_assert(offsetof(CScriptThing_MsgHowLongWasExpressionPerformed_ThisOverlay, m_Field04) == 0x04);

struct CScriptThing_MsgHowLongWasExpressionPerformed_TargetOverlay
{
    void** m_Vftable; // 0x00
};
static_assert(offsetof(CScriptThing_MsgHowLongWasExpressionPerformed_TargetOverlay, m_Vftable) == 0x00);
}

long __thiscall CScriptThing::MsgHowLongWasExpressionPerformed() const
{
    const auto* const self =
        reinterpret_cast<const CScriptThing_MsgHowLongWasExpressionPerformed_ThisOverlay*>(this);
    auto* const target =
        reinterpret_cast<CScriptThing_MsgHowLongWasExpressionPerformed_TargetOverlay*>(self->m_Field04);

    if (target == nullptr)
    {
        return 0;
    }

    using IndirectFn = long(__thiscall*)(void*);
    const auto fn = reinterpret_cast<IndirectFn>(target->m_Vftable[0x7C / sizeof(void*)]);
    return fn(target);
}