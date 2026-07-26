class CGameScriptInterface
{
public:
    void __thiscall SetGlobalTargetingDistanceOffset(float targetingDistanceOffset);
};

namespace
{
float& DAT_013bae28 = *reinterpret_cast<float*>(0x013BAE28);
}

void __thiscall CGameScriptInterface::SetGlobalTargetingDistanceOffset(float targetingDistanceOffset)
{
    DAT_013bae28 = targetingDistanceOffset;
}