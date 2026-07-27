#include <cstddef>
#include <cstdint>
#include <windows.h>

struct CritSec_GetDividedValue_Locked_Overlay
{
    std::int32_t field_0000;
    std::byte pad_0004[0x60];
    CRITICAL_SECTION field_0064;
    std::byte pad_007c[0x90];
    std::int32_t field_010c;
    std::uint32_t field_0110;
    std::uint32_t field_0114;
};

static_assert(offsetof(CritSec_GetDividedValue_Locked_Overlay, field_0000) == 0x00);
static_assert(offsetof(CritSec_GetDividedValue_Locked_Overlay, field_0064) == 0x64);
static_assert(offsetof(CritSec_GetDividedValue_Locked_Overlay, field_010c) == 0x10C);
static_assert(offsetof(CritSec_GetDividedValue_Locked_Overlay, field_0110) == 0x110);
static_assert(offsetof(CritSec_GetDividedValue_Locked_Overlay, field_0114) == 0x114);

extern "C" std::int64 __cdecl __alldiv(
    std::uint32_t dividend_low,
    std::uint32_t dividend_high,
    std::int32_t divisor_low,
    std::int32_t divisor_high);

std::uint32_t __stdcall CritSec_GetDividedValue_Locked(void* param_1, std::uint32_t* param_2)
{
    if (param_2 == nullptr)
    {
        return 0x80004003;
    }

    auto* const self = reinterpret_cast<CritSec_GetDividedValue_Locked_Overlay*>(
        reinterpret_cast<std::uintptr_t>(param_1) - 0xC8);
    LPCRITICAL_SECTION const lpCriticalSection = &self->field_0064;

    EnterCriticalSection(lpCriticalSection);

    if ((self->field_0000 != 0) && (1 < self->field_010c))
    {
        const std::int32_t iVar1 = self->field_010c - 1;
        *param_2 = static_cast<std::uint32_t>(__alldiv(
            self->field_0110,
            self->field_0114,
            iVar1,
            iVar1 >> 0x1F));
        LeaveCriticalSection(lpCriticalSection);
        return 0;
    }

    *param_2 = 0;
    LeaveCriticalSection(lpCriticalSection);
    return 0;
}