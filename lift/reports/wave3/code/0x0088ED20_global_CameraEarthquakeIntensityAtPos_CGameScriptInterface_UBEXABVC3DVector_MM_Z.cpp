#include <cstdint>
#include <cstring>

struct C3DVector
{
    float x;
    float y;
    float z;
};

class CGameScriptInterface;

class CTCDRumble
{
public:
    static void Create(...);
};

namespace
{
inline const float& _DAT_0122DED8 = *reinterpret_cast<const float*>(0x0122DED8);
inline const float& _DAT_0122DEDC = *reinterpret_cast<const float*>(0x0122DEDC);
inline const float& _DAT_01230A08 = *reinterpret_cast<const float*>(0x01230A08);

inline std::uint8_t SUB41(float value, unsigned int)
{
    std::uint8_t result;
    std::memcpy(&result, &value, sizeof(result));
    return result;
}
}

void __fastcall CameraEarthquakeIntensityAtPos_CGameScriptInterface_0088ED20(
    const CGameScriptInterface*,
    std::uint32_t param_2,
    const C3DVector& param_3,
    float param_4,
    float param_5)
{
    if (_DAT_0122DEDC <= param_4)
    {
        if (_DAT_0122DED8 < param_4)
        {
            param_4 = 1.0f;
        }
    }
    else
    {
        param_4 = 0.0f;
    }

    // Pseudo-C is treated as ground truth here despite the byte-stream mismatch on this store.
    if (_DAT_0122DEDC <= param_5)
    {
        if (_DAT_01230A08 < param_5)
        {
            param_5 = 0.0f;
        }
    }
    else
    {
        param_5 = 0.0f;
    }

    CTCDRumble::Create(
        &param_3,
        static_cast<float>(param_2 & 0xFFFFFF00u),
        param_4,
        SUB41(param_5, 0));
}