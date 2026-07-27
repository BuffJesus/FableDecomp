#pragma once

#include "rebuild_abi.h"

enum FableFrontendPostMovieStep
{
    FableFrontendOpenFrontendBank = 0,
    FableFrontendOpenTextBank = 1,
    FableFrontendOpenShadersBank = 2,
    FableFrontendInit2 = 3,
    FableFrontendInitialiseEngine = 4,
    FableFrontendInitialiseFrontend = 5,
    FableFrontendClearRenderTarget = 6,
    FableFrontendSwapScreens = 7,
    FableFrontendChangeStateFirstTime = 8
};

typedef bool (FABLE_FASTCALL *FableFrontendPostMovieAction)(
    void* context,
    FableFrontendPostMovieStep step);

struct FableFrontendPostMovieServices
{
    bool openRetailBanks;
    FableFrontendPostMovieAction action;
    void* context;
};

// Authored integration seam for the recovered post-movie prefix of
// CNewFrontendGameComponent::Run @ 0x0042EC7C. Each action remains an
// explicit boundary until its retail owner is reconstructed.
bool FABLE_FASTCALL FableRunFrontendPostMovieStartup(
    const FableFrontendPostMovieServices& services);
