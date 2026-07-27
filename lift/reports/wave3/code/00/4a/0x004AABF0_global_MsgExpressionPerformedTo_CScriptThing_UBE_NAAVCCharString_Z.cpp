#include <cstddef>

struct CScriptThing_MsgExpressionPerformedTo_Overlay {
    std::byte pad_0[0x4];
    void* object_at_0x4;
};
static_assert(offsetof(CScriptThing_MsgExpressionPerformedTo_Overlay, object_at_0x4) == 0x4);

bool CScriptThing::MsgExpressionPerformedTo(CCharString& string)
{
    auto* const self = reinterpret_cast<CScriptThing_MsgExpressionPerformedTo_Overlay*>(this);
    void* const target = self->object_at_0x4;
    if (target == nullptr) {
        return false;
    }

    const auto vftable = *reinterpret_cast<void***>(target);
    const auto fn =
        reinterpret_cast<bool(__thiscall*)(void*, CCharString&)>(vftable[0x74 / sizeof(void*)]);
    return fn(target, string);
}