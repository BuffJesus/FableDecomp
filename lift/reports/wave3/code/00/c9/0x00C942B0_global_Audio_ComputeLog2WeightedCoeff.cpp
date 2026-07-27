#include <cstdint>

extern float DAT_013a3d48[];
extern float DAT_013a3948[];
extern float _DAT_0122ded8;

// The original routine consumes caller-provided values in ESI/EDI.
// They are made explicit here because no PDB/source prototype is available.
void Audio_ComputeLog2WeightedCoeff(
    float param_1,
    float* param_2,
    std::uint32_t* param_3,
    std::uint32_t unaff_ESI,
    std::uint32_t unaff_EDI)
{
    const std::uint32_t sign = static_cast<std::uint32_t>(
        static_cast<std::int32_t>(unaff_EDI - unaff_ESI) >> 31);

    const std::uint32_t selectMask =
        static_cast<std::uint32_t>(
            (static_cast<std::int32_t>(unaff_ESI) < static_cast<std::int32_t>(unaff_EDI)) - 1);

    std::uint32_t log2Index =
        0x1Fu - (((unaff_EDI - unaff_ESI) ^ sign) - sign);

    log2Index =
        static_cast<std::uint32_t>(
            (static_cast<std::int32_t>(log2Index) < 0) - 1) & log2Index;

    *param_2 =
        (DAT_013a3d48[log2Index] + _DAT_0122ded8) *
        DAT_013a3948[(~selectMask & unaff_EDI) | (selectMask & unaff_ESI)] *
        param_1;

    *param_3 = 0;
}