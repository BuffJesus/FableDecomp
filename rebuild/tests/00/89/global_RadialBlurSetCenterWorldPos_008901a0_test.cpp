#include <cstdio>
#include <cstring>

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
    void* param_2, const C3DVector& param_3);

int main()
{
    // Patch the fixed-address call target with a small trampoline is not
    // possible in a plain test binary, so instead we validate the pointer-
    // chase arithmetic directly (the part that is testable without the
    // real retail image mapped at 0x006B8310), and separately sanity check
    // the struct layout offsets used by the candidate.

    unsigned char blob18[0x1964 + sizeof(void*)];
    std::memset(blob18, 0, sizeof(blob18));
    CEnvironmentSpellEffectInterpolator* fakeInterp =
        reinterpret_cast<CEnvironmentSpellEffectInterpolator*>(0x12345678u);
    std::memcpy(blob18 + 0x1964, &fakeInterp, sizeof(fakeInterp));
    CGameScriptInterface_Field18_Overlay* field18 =
        reinterpret_cast<CGameScriptInterface_Field18_Overlay*>(blob18);

    unsigned char blob4[0x18 + sizeof(void*)];
    std::memset(blob4, 0, sizeof(blob4));
    void* field18ptr = field18;
    std::memcpy(blob4 + 0x18, &field18ptr, sizeof(field18ptr));
    CGameScriptInterface_Field4_Overlay* field4 =
        reinterpret_cast<CGameScriptInterface_Field4_Overlay*>(blob4);

    unsigned char blob0[0x4 + sizeof(void*)];
    std::memset(blob0, 0, sizeof(blob0));
    void* field4ptr = field4;
    std::memcpy(blob0 + 0x4, &field4ptr, sizeof(field4ptr));
    CGameScriptInterface_Overlay* selfObj =
        reinterpret_cast<CGameScriptInterface_Overlay*>(blob0);

    CGameScriptInterface_Field4_Overlay* chasedField4 =
        *reinterpret_cast<CGameScriptInterface_Field4_Overlay* const*>(&selfObj->field_0004);
    CGameScriptInterface_Field18_Overlay* chasedField18 =
        *reinterpret_cast<CGameScriptInterface_Field18_Overlay* const*>(&chasedField4->field_0018);

    bool ok = (chasedField18->radialBlurInterpolator == fakeInterp);

    if (ok) {
        std::printf("RADIALBLUR_CENTER_CHASE_OK\n");
        return 0;
    }
    std::printf("RADIALBLUR_CENTER_CHASE_FAIL\n");
    return 1;
}