struct CGameScriptInterface_Overlay
{
    std::byte pad_0000[0x4];
    void* field_0004;
};
static_assert(offsetof(CGameScriptInterface_Overlay, field_0004) == 0x4);

struct CGameScriptInterface_Field4_Overlay
{
    std::byte pad_0000[0x18];
    void* field_0018;
};
static_assert(offsetof(CGameScriptInterface_Field4_Overlay, field_0018) == 0x18);

struct CGameScriptInterface_Field18_Overlay
{
    std::byte pad_0000[0x1964];
    CEnvironmentSpellEffectInterpolator* radialBlurInterpolator;
};
static_assert(offsetof(CGameScriptInterface_Field18_Overlay, radialBlurInterpolator) == 0x1964);

void CGameScriptInterface::RadialBlurSetCenterWorldPos(void* param_2, const C3DVector& param_3)
{
    auto* const self = reinterpret_cast<CGameScriptInterface_Overlay*>(this);
    auto* const field4 =
        *reinterpret_cast<CGameScriptInterface_Field4_Overlay* const*>(&self->field_0004);
    auto* const field18 =
        *reinterpret_cast<CGameScriptInterface_Field18_Overlay* const*>(&field4->field_0018);

    CEnvironmentSpellEffectInterpolator::RadialBlurSetCenterWorldPos(
        field18->radialBlurInterpolator,
        param_2,
        &param_3);
}