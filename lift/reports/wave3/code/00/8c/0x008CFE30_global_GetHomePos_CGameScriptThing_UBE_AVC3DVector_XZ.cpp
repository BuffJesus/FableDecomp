#include <cstddef>

struct C3DVector
{
    float x;
    float y;
    float z;
};
static_assert(sizeof(C3DVector) == 0x0C);

class CGameScriptThing
{
public:
    C3DVector __thiscall GetHomePos() const;
};

namespace
{
struct CGameScriptThing_GetHomePos_ThisOverlay
{
    std::byte m_Pad00[0x34];
    C3DVector m_Field34; // 0x34
};
static_assert(offsetof(CGameScriptThing_GetHomePos_ThisOverlay, m_Field34) == 0x34);
}

C3DVector __thiscall CGameScriptThing::GetHomePos() const
{
    const auto* const self =
        reinterpret_cast<const CGameScriptThing_GetHomePos_ThisOverlay*>(this);

    return self->m_Field34;
}