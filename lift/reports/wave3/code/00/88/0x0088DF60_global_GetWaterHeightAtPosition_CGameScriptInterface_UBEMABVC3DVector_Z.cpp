#include <cstddef>
#include <cstdint>

class C3DVector;

class CWorldMap
{
public:
    float GetWaterSizeZAt(const C3DVector& position) const;
};

class CGameScriptInterface
{
public:
    float GetWaterHeightAtPosition(const C3DVector& position) const;
};

struct CGameScriptInterface_Overlay
{
    std::uint8_t pad_0x00[0x04];
    void* m_UnknownAt0x04; // 0x04
};

static_assert(offsetof(CGameScriptInterface_Overlay, m_UnknownAt0x04) == 0x04);
static_assert(sizeof(CGameScriptInterface_Overlay) == 0x08);

float CGameScriptInterface::GetWaterHeightAtPosition(const C3DVector& position) const
{
    using Slot034Fn = CWorldMap* (__thiscall*)(void*, const C3DVector*);

    const auto* const self = reinterpret_cast<const CGameScriptInterface_Overlay*>(this);
    void* const interfaceObject = self->m_UnknownAt0x04;
    void* const* const vtable = *reinterpret_cast<void* const* const*>(interfaceObject);
    const auto slot034 =
        reinterpret_cast<const Slot034Fn*>(reinterpret_cast<const std::uint8_t*>(vtable) + 0x34);

    CWorldMap* const worldMap = (*slot034)(interfaceObject, &position);
    return worldMap->GetWaterSizeZAt(position);
}