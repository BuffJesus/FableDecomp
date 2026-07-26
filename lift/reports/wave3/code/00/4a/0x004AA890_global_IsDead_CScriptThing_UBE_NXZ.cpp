#include <cstddef>
#include <cstdint>

class CScriptThing;

namespace
{
struct CScriptThing_VTable
{
    std::byte m_Pad00[0x12C];
    std::uint8_t (__thiscall* m_UnknownAt12C)(const CScriptThing*); // 0x12C
};
static_assert(offsetof(CScriptThing_VTable, m_UnknownAt12C) == 0x12C);
}

class CScriptThing
{
public:
    bool __thiscall IsDead() const;
};

bool CScriptThing::IsDead() const
{
    const auto* const vftable = *reinterpret_cast<CScriptThing_VTable* const*>(this);
    const std::uint8_t result = vftable->m_UnknownAt12C(this);
    return result == 0;
}