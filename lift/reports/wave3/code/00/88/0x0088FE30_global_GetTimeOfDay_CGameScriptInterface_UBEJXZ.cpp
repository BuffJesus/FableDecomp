#include <cstddef>
#include <cstdint>

class CGameScriptInterface
{
public:
    long __thiscall GetTimeOfDay() const;
};

extern "C" void TimeOfDay_GetRoundedHour();

namespace
{
struct CGameScriptInterface_TimeOfDayOverlay
{
    std::byte pad_0[0x4];
    void* pTimeManager;
};
static_assert(offsetof(CGameScriptInterface_TimeOfDayOverlay, pTimeManager) == 0x4);

struct TimeManager_TimeOfDayOverlay
{
    std::byte pad_0[0x1C];
    void* pTimeOfDay;
};
static_assert(offsetof(TimeManager_TimeOfDayOverlay, pTimeOfDay) == 0x1C);
} // namespace

__declspec(naked) long __thiscall CGameScriptInterface::GetTimeOfDay() const
{
    // Tailcall thunk:
    //   eax = [ecx + 0x4]
    //   ecx = [eax + 0x1C]
    //   jmp TimeOfDay_GetRoundedHour
    __asm
    {
        mov eax, [ecx + 0x4]
        mov ecx, [eax + 0x1C]
        jmp TimeOfDay_GetRoundedHour
    }
}