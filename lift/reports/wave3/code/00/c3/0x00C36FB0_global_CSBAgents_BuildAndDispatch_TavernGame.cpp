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

struct TavernGameDispatchTarget_Overlay
{
    std::uint8_t pad00[0x10];
    void** vftable10; // +0x10
};

struct CSBAgentsCtorObservedResult
{
    std::uintptr_t edxAfterCall;
};

using DispatchFn = void(__thiscall*)(void* self, CSBAgents* agents);

extern CSBAgentsCtorObservedResult __fastcall Call_CSBAgents_ctor_Observed(
    CSBAgents* self,
    CSteeringAgent* agent,
    SteeringAgentVector* agents);

static_assert(sizeof(CSBAgents) == 0x08);
static_assert(offsetof(TavernGameDispatchTarget_Overlay, vftable10) == 0x10);

static void __fastcall CSBAgents_BuildAndDispatch_TavernGame_Impl(
    CSteeringAgent* agent,
    void*,
    SteeringAgentVector* agents)
{
    CSBAgents builtAgents;
    const CSBAgentsCtorObservedResult ctorResult =
        Call_CSBAgents_ctor_Observed(&builtAgents, agent, agents);

    auto* const dispatchTarget =
        reinterpret_cast<TavernGameDispatchTarget_Overlay*>(ctorResult.edxAfterCall);

    const auto dispatch =
        reinterpret_cast<DispatchFn>(dispatchTarget->vftable10[0x28 / sizeof(void*)]);

    dispatch(
        reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(dispatchTarget) + 0x10),
        &builtAgents);
}
}

__declspec(naked) void __stdcall CSBAgents_BuildAndDispatch_TavernGame(
    SteeringAgentVector* agents)
{
    __asm
    {
        jmp CSBAgents_BuildAndDispatch_TavernGame_Impl
    }
}