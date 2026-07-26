#include <cstddef>
#include <cstdint>
#include <vector>

struct CSteeringAgent;

namespace
{
using SteeringAgentVector =
    std::vector<CSteeringAgent const*, std::allocator<CSteeringAgent const*>>;

struct CSBAgents
{
    std::uint8_t bytes[0x08];
};

struct DispatchOuter_Overlay
{
    std::uint8_t pad00[0x04];
    void* object04; // +0x04
};

struct DispatchInner_Overlay
{
    std::uint8_t pad00[0x08];
    void** vftable; // +0x08
};

struct CSBAgentsCtorObservedResult
{
    std::uintptr_t edxAfterCall;
};

using DispatchFn = void(__thiscall*)(void* self, CSBAgents* agents);

extern void __thiscall CSBAgents_ctor(CSBAgents* self, SteeringAgentVector* agents);
extern CSBAgentsCtorObservedResult __cdecl Call_CSBAgents_ctor_Observed(
    CSBAgents* self,
    SteeringAgentVector* agents);

static_assert(sizeof(CSBAgents) == 0x08);
static_assert(offsetof(DispatchOuter_Overlay, object04) == 0x04);
static_assert(offsetof(DispatchInner_Overlay, vftable) == 0x08);
}

void __fastcall CSBAgents_BuildAndDispatch_A(SteeringAgentVector* agents)
{
    CSBAgents builtAgents;
    const CSBAgentsCtorObservedResult ctorResult =
        Call_CSBAgents_ctor_Observed(&builtAgents, agents);

    auto* const dispatchOuter =
        reinterpret_cast<DispatchOuter_Overlay*>(ctorResult.edxAfterCall);
    auto* const dispatchInner =
        reinterpret_cast<DispatchInner_Overlay*>(dispatchOuter->object04);

    const auto dispatch =
        reinterpret_cast<DispatchFn>(dispatchInner->vftable[0x28 / sizeof(void*)]);

    dispatch(
        reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(dispatchInner) + 0x08),
        &builtAgents);
}