#include <cstddef>
#include <cstdint>

class C3DVector;
class CGameScriptInterface;

namespace
{
struct CGameScriptInterface_IsCameraPosOnScreen_SelfOverlay
{
    void* m_Vftable; // 0x00
    void* m_Field04; // 0x04
};
static_assert(offsetof(CGameScriptInterface_IsCameraPosOnScreen_SelfOverlay, m_Field04) == 0x04);

struct CGameScriptInterface_IsCameraPosOnScreen_Field04Overlay
{
    std::byte m_Pad00[0x34];
    void* m_Field34; // 0x34
};
static_assert(offsetof(CGameScriptInterface_IsCameraPosOnScreen_Field04Overlay, m_Field34) == 0x34);

using FirstCallType = bool(__thiscall*)(void* self, CGameScriptInterface** interfaceValue);
using SecondCallType = bool(__thiscall*)(CGameScriptInterface* self, const C3DVector& cameraPos);
}

class CGameScriptInterface
{
public:
    bool __thiscall IsCameraPosOnScreen(const C3DVector& cameraPos) const
    {
        auto* const self =
            reinterpret_cast<const CGameScriptInterface_IsCameraPosOnScreen_SelfOverlay*>(this);
        auto* const field04 =
            reinterpret_cast<const CGameScriptInterface_IsCameraPosOnScreen_Field04Overlay*>(
                self->m_Field04);
        void* const field34 = field04->m_Field34;

        CGameScriptInterface* interfaceValue = const_cast<CGameScriptInterface*>(this);

        const auto firstVftable = *reinterpret_cast<void***>(field34);
        const auto firstCall =
            reinterpret_cast<FirstCallType>(firstVftable[0x28 / sizeof(void*)]);
        if (firstCall(field34, &interfaceValue))
        {
            const auto secondVftable =
                *reinterpret_cast<void***>(interfaceValue);
            const auto secondCall =
                reinterpret_cast<SecondCallType>(secondVftable[0x0C / sizeof(void*)]);
            if (secondCall(interfaceValue, cameraPos))
            {
                return true;
            }
        }

        return false;
    }
};