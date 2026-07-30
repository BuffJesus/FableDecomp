#include "fable_gfmain.h"
#include "fable_movie.h"
#include "fable_profile.h"
#include "fable_string.h"

unsigned long CEngineInternalPrimitiveBase_AddChildPrimitive();

long FABLE_FASTCALL FableRunGFMainPhase9(
    GFMainPhase9State& state)
{
    state.configuredEngineFlags = state.initialEngineFlags;
    if (state.engineFlagSourceA)
        state.configuredEngineFlags |= 1;
    if (!state.engineFlagSourceB)
        state.configuredEngineFlags |= 2;

    if (state.forceMovieLeftAlign)
    {
        CXMVPlayer::SetLeftAlignText(true);
        state.movieLeftAlignApplied = true;
    }

    void* systemManager =
        FableGFMainPhase9GetSystemManagerBoundary(state);
    state.systemManagerToken =
        static_cast<fable_u32>(
            reinterpret_cast<size_t>(systemManager));
    state.observedSystemInitialiseResult =
        FableGFMainPhase9InitialiseSystemManagerBoundary(
            systemManager,
            state.configuredEngineFlags,
            state);

    NProfileTimer::EndProfile(0);

    systemManager =
        FableGFMainPhase9GetSystemManagerBoundary(state);
    state.secondSystemManagerToken =
        static_cast<fable_u32>(
            reinterpret_cast<size_t>(systemManager));
    state.imePrimitiveToken =
        static_cast<fable_u32>(
            CEngineInternalPrimitiveBase_AddChildPrimitive());
    FableGFMainPhase9InitialiseImeBoundary(
        reinterpret_cast<void*>(
            static_cast<size_t>(state.imePrimitiveToken)),
        reinterpret_cast<void*>(
            static_cast<size_t>(state.systemWindowHandleToken)),
        state);

    {
        CCharString profile("End basic init", -1);
        NProfileTimer::StartProfile(profile, 0);
    }

    if (state.processSaveFileMetadata)
        FableGFMainPhase9ProcessSaveMetadataBoundary(state);

    return 0;
}
