#include <bit>
#include <cstddef>
#include <cstdint>

namespace
{
struct Math_PolygonAngleSumClosureCheck_Vec3Overlay
{
    float x; // +0x00
    float y; // +0x04
    float z; // +0x08
};
static_assert(sizeof(Math_PolygonAngleSumClosureCheck_Vec3Overlay) == 0x0C);
static_assert(offsetof(Math_PolygonAngleSumClosureCheck_Vec3Overlay, x) == 0x00);
static_assert(offsetof(Math_PolygonAngleSumClosureCheck_Vec3Overlay, y) == 0x04);
static_assert(offsetof(Math_PolygonAngleSumClosureCheck_Vec3Overlay, z) == 0x08);

struct Math_PolygonAngleSumClosureCheck_PointListOverlay
{
    Math_PolygonAngleSumClosureCheck_Vec3Overlay* begin; // +0x00
    Math_PolygonAngleSumClosureCheck_Vec3Overlay* end;   // +0x04
};
static_assert(offsetof(Math_PolygonAngleSumClosureCheck_PointListOverlay, begin) == 0x00);
static_assert(offsetof(Math_PolygonAngleSumClosureCheck_PointListOverlay, end) == 0x04);

extern "C" void __cdecl _CIacos();

[[nodiscard]] static float Math_PolygonAngleSumClosureCheck_InvSqrtOrdered(const float value)
{
    const std::uint32_t bits = std::bit_cast<std::uint32_t>(value);
    const std::uint32_t mantissaIndex = (bits >> 17U) & 0x7FU;
    const std::uint32_t exponentIndex = (bits >> 23U) & 0xFFU;

    const auto* const inverseSqrtMantissaTable =
        reinterpret_cast<const std::uint32_t*>(0x013CE558);

    const std::uint32_t mantissaBits = inverseSqrtMantissaTable[mantissaIndex] << 15U;
    const std::uint32_t exponentBits =
        ((0x17CU - exponentIndex) << 22U) & 0xFF800000U;
    const float estimate = std::bit_cast<float>(mantissaBits | exponentBits);

    const float three = *reinterpret_cast<const float*>(0x0122DED4);
    const float oneHalf = *reinterpret_cast<const float*>(0x0122F59C);

    const float estimateSquared = estimate * estimate;
    const float scaledValue = value * estimateSquared;
    const float correction = three - scaledValue;
    const float scaledEstimate = correction * estimate;
    return scaledEstimate * oneHalf;
}

[[nodiscard]] static float Math_PolygonAngleSumClosureCheck_CornerDot(
    const Math_PolygonAngleSumClosureCheck_Vec3Overlay& previous,
    const Math_PolygonAngleSumClosureCheck_Vec3Overlay& vertex,
    const Math_PolygonAngleSumClosureCheck_Vec3Overlay& next)
{
    const float ax = previous.x - vertex.x;
    const float ay = previous.y - vertex.y;
    const float az = previous.z - vertex.z;

    const float bx = next.x - vertex.x;
    const float by = next.y - vertex.y;
    const float bz = next.z - vertex.z;

    const float lenASquaredXY = (ay * ay) + (ax * ax);
    const float lenASquared = (az * az) + lenASquaredXY;
    const float invLenA = Math_PolygonAngleSumClosureCheck_InvSqrtOrdered(lenASquared);

    const float lenBSquaredXY = (by * by) + (bx * bx);
    const float lenBSquared = (bz * bz) + lenBSquaredXY;
    const float invLenB = Math_PolygonAngleSumClosureCheck_InvSqrtOrdered(lenBSquared);

    const float zTerm = (az * bz) * invLenA * invLenB;
    const float yTerm = (ay * by) * invLenA * invLenB;
    const float zySum = zTerm + yTerm;
    const float xTerm = (ax * bx) * invLenA * invLenB;
    return zySum + xTerm;
}
}

std::uint32_t __fastcall Math_PolygonAngleSumClosureCheck(
    const Math_PolygonAngleSumClosureCheck_PointListOverlay* pointList)
{
    Math_PolygonAngleSumClosureCheck_Vec3Overlay* previous = pointList->begin;
    Math_PolygonAngleSumClosureCheck_Vec3Overlay* vertex = pointList->begin + 1;
    Math_PolygonAngleSumClosureCheck_Vec3Overlay* current = pointList->begin + 2;
    Math_PolygonAngleSumClosureCheck_Vec3Overlay* const end = pointList->end;

    float accumulatedAngleDegrees = 0.0f;

    const float radiansToDegrees = *reinterpret_cast<const float*>(0x01237F08);
    const float expectedAngleSum = *reinterpret_cast<const float*>(0x0122DED8);
    const float epsilon = *reinterpret_cast<const float*>(0x0126A200);

    for (; current != end; ++current)
    {
        const float normalizedDot =
            Math_PolygonAngleSumClosureCheck_CornerDot(*previous, *vertex, *current);

        __asm
        {
            fld dword ptr [normalizedDot]
            call _CIacos
            fmul dword ptr [radiansToDegrees]
            fadd dword ptr [accumulatedAngleDegrees]
            fstp dword ptr [accumulatedAngleDegrees]
        }

        previous = vertex;
        vertex = current;
    }

    const float tailDot0 =
        Math_PolygonAngleSumClosureCheck_CornerDot(*previous, *vertex, *pointList->begin);
    const float tailDot1 =
        Math_PolygonAngleSumClosureCheck_CornerDot(*vertex, *pointList->begin, pointList->begin[1]);

    std::uint32_t result;

    __asm
    {
        fld dword ptr [tailDot0]
        call _CIacos
        fld dword ptr [tailDot1]
        call _CIacos
        fmul dword ptr [radiansToDegrees]
        fadd dword ptr [accumulatedAngleDegrees]
        faddp st(1), st(0)
        fsub dword ptr [expectedAngleSum]
        fld st(0)
        fmul st(0), st(1)
        fcomp dword ptr [epsilon]
        fnstsw ax
        fstp st(0)
        test ah, 5
        jp return_zero

        mov eax, 1
        jmp store_result

    return_zero:
        xor eax, eax

    store_result:
        mov result, eax
    }

    return result;
}