#include <cstdint>

class CGameScriptInterface
{
public:
    virtual float __thiscall GetGlobalTargetingDistanceOffset();
};

float __thiscall CGameScriptInterface::GetGlobalTargetingDistanceOffset()
{
    return *reinterpret_cast<const float*>(static_cast<std::uintptr_t>(0x013BAE28));
}