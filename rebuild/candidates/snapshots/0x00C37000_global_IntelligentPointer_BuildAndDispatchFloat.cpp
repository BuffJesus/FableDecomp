#include <cstddef>
#include <cstdint>

namespace
{
struct CBaseIntelligentPointer
{
    std::uint8_t bytes[0x08];
};

struct IntelligentPointerDispatchTarget_Overlay
{
    std::uint8_t pad00[0x10];
    void** vftable10; // +0x10
};

struct CBaseIntelligentPointerCtorObservedResult
{
    std::uintptr_t edxAfterCall;
};

using DispatchFn = void(__thiscall*)(void* self, CBaseIntelligentPointer* value);

extern CBaseIntelligentPointerCtorObservedResult __fastcall
Call_CBaseIntelligentPointer_ctor_Observed(
    CBaseIntelligentPointer* self,
    void* ctorArg);

static_assert(sizeof(CBaseIntelligentPointer) == 0x08);
static_assert(
    offsetof(IntelligentPointerDispatchTarget_Overlay, vftable10) == 0x10);
}

float __fastcall IntelligentPointer_BuildAndDispatchFloat(void* ctorArg)
{
    CBaseIntelligentPointer builtValue;
    const CBaseIntelligentPointerCtorObservedResult ctorResult =
        Call_CBaseIntelligentPointer_ctor_Observed(&builtValue, ctorArg);

    auto* const dispatchTarget =
        reinterpret_cast<IntelligentPointerDispatchTarget_Overlay*>(
            ctorResult.edxAfterCall);

    const auto dispatch =
        reinterpret_cast<DispatchFn>(
            dispatchTarget->vftable10[0x28 / sizeof(void*)]);

    dispatch(
        reinterpret_cast<void*>(
            reinterpret_cast<std::uintptr_t>(dispatchTarget) + 0x10),
        &builtValue);

    return *reinterpret_cast<float*>(&builtValue);
}