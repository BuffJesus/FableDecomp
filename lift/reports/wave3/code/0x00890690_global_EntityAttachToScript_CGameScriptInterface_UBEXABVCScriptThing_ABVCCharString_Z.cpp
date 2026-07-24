#include <cstddef>
#include <cstdint>

class CCharString;
class CScriptThing;

class CGameScriptInterface
{
public:
    void __thiscall EntityAttachToScript(
        const CScriptThing& scriptThing,
        const CCharString& string) const;
};

extern "C" CCharString* __thiscall CCharString__operator_equals(
    CCharString* pThis,
    const CCharString* pOther);

namespace
{
using EntityAttachToScript_IndirectFn = void* (__thiscall*)(const CGameScriptInterface* pThis);

struct EntityAttachToScript_Result_Field58_Overlay
{
    std::byte m_Pad00[0x58];
};
static_assert(offsetof(EntityAttachToScript_Result_Field58_Overlay, m_Pad00) == 0x0);

struct EntityAttachToScript_Result_Field91_Overlay
{
    std::byte m_Pad00[0x91];
    std::uint8_t m_Field91;
};
static_assert(offsetof(EntityAttachToScript_Result_Field91_Overlay, m_Field91) == 0x91);
}

void __thiscall CGameScriptInterface::EntityAttachToScript(
    const CScriptThing& scriptThing,
    const CCharString& string) const
{
    (void)scriptThing;

    void* const pResult =
        (*reinterpret_cast<EntityAttachToScript_IndirectFn const*>(
            *reinterpret_cast<std::uintptr_t const*>(this) + 0x2C))(this);

    if (pResult != nullptr &&
        (reinterpret_cast<EntityAttachToScript_Result_Field91_Overlay*>(pResult)->m_Field91 & 0x1) == 0) {
        CCharString__operator_equals(
            reinterpret_cast<CCharString*>(
                &reinterpret_cast<EntityAttachToScript_Result_Field58_Overlay*>(pResult)->m_Pad00[0x58]),
            &string);
    }
}