#include "frontend_startup_sequence.h"

namespace
{
    bool RunStep(
        const FableFrontendPostMovieServices& services,
        FableFrontendPostMovieStep step)
    {
        return
            services.action != 0 &&
            services.action(services.context, step);
    }
}

bool FABLE_FASTCALL FableRunFrontendPostMovieStartup(
    const FableFrontendPostMovieServices& services)
{
    if (services.openRetailBanks)
    {
        if (!RunStep(services, FableFrontendOpenFrontendBank))
            return false;
        if (!RunStep(services, FableFrontendOpenTextBank))
            return false;
        if (!RunStep(services, FableFrontendOpenShadersBank))
            return false;
    }

    if (!RunStep(services, FableFrontendInit2))
        return false;
    if (!RunStep(services, FableFrontendInitialiseEngine))
        return false;
    if (!RunStep(services, FableFrontendInitialiseFrontend))
        return false;
    if (!RunStep(services, FableFrontendClearRenderTarget))
        return false;
    if (!RunStep(services, FableFrontendSwapScreens))
        return false;
    return RunStep(
        services,
        FableFrontendChangeStateFirstTime);
}
