// CGameScriptInterface::RadialBlurSetCenterWorldPos(void*, const C3DVector&) @ 0x008901A0.
//
// Genuine C++ tail-call thunk: chase this->field_0004->field_0018->field_1964
// to reach the CEnvironmentSpellEffectInterpolator instance, then forward the
// call (same ecx-this, same stack args) to that object's own
// RadialBlurSetCenterWorldPos member (real retail impl at fixed address
// 0x006B8310). CEnvironmentSpellEffectInterpolator::RadialBlurSetCenterWorldPos
// is DECLARED but not DEFINED in this TU, so the compiler cannot inline it and
// -- because the call is this function's last statement with a compatible
// (__fastcall/void) signature -- VC7.1 tail-call-optimizes the call into a
// single relative jmp (relocation-masked in parity) instead of a call+ret,
// reproducing retail's 12-byte pointer chase followed by `e9 xx xx xx xx`.

struct C3DVector { float x, y, z; };

class CEnvironmentSpellEffectInterpolator
{
public:
    void RadialBlurSetCenterWorldPos(void* param_2, const C3DVector& param_3);
};

struct CGameScriptInterface_Overlay
{
    unsigned char pad_0000[0x4];
    void* field_0004;
};

struct CGameScriptInterface_Field4_Overlay
{
    unsigned char pad_0000[0x18];
    void* field_0018;
};

struct CGameScriptInterface_Field18_Overlay
{
    unsigned char pad_0000[0x1964];
    CEnvironmentSpellEffectInterpolator* radialBlurInterpolator;
};

extern "C" void __fastcall
CGameScriptInterface_RadialBlurSetCenterWorldPos(
    CGameScriptInterface_Overlay* self, void* edxDummy,
    void* worldPosArg, const C3DVector& worldPosVec)
{
    CGameScriptInterface_Field4_Overlay* field4 =
        *reinterpret_cast<CGameScriptInterface_Field4_Overlay* const*>(&self->field_0004);
    CGameScriptInterface_Field18_Overlay* field18 =
        *reinterpret_cast<CGameScriptInterface_Field18_Overlay* const*>(&field4->field_0018);

    field18->radialBlurInterpolator->RadialBlurSetCenterWorldPos(worldPosArg, worldPosVec);
}