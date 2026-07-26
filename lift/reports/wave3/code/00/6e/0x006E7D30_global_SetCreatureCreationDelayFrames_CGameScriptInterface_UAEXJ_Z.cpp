#include <cstddef>

class CGameScriptInterface
{
public:
    virtual void __thiscall SetCreatureCreationDelayFrames(long delayFrames);
};

namespace
{
struct CGameScriptInterface_Overlay
{
    std::byte m_Pad00[0x4C];
    long m_Field4C;
};

static_assert(offsetof(CGameScriptInterface_Overlay, m_Field4C) == 0x4C);
}

void __thiscall CGameScriptInterface::SetCreatureCreationDelayFrames(long delayFrames)
{
    reinterpret_cast<CGameScriptInterface_Overlay*>(this)->m_Field4C = delayFrames;
}