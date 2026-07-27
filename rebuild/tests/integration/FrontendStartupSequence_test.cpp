#include "../../integration/frontend_startup_sequence.h"

#include <stdio.h>

namespace
{
    struct SequenceContext
    {
        FableFrontendPostMovieStep observed[9];
        unsigned long count;
        int failAt;
    };

    bool FABLE_FASTCALL RecordStep(
        void* opaqueContext,
        FableFrontendPostMovieStep step)
    {
        SequenceContext* const context =
            static_cast<SequenceContext*>(opaqueContext);
        if (context->count >= 9)
            return false;
        context->observed[context->count++] = step;
        return static_cast<int>(step) != context->failAt;
    }

    bool IsCompleteRetailSequence(
        const SequenceContext& context)
    {
        if (context.count != 9)
            return false;
        for (unsigned long index = 0; index < 9; ++index)
        {
            if (
                context.observed[index] !=
                static_cast<FableFrontendPostMovieStep>(index))
            {
                return false;
            }
        }
        return true;
    }
}

int main()
{
    SequenceContext retail = {};
    retail.failAt = -1;
    FableFrontendPostMovieServices retailServices = {
        true,
        &RecordStep,
        &retail
    };
    if (
        !FableRunFrontendPostMovieStartup(retailServices) ||
        !IsCompleteRetailSequence(retail))
    {
        return 1;
    }

    SequenceContext noBanks = {};
    noBanks.failAt = -1;
    FableFrontendPostMovieServices noBankServices = {
        false,
        &RecordStep,
        &noBanks
    };
    if (
        !FableRunFrontendPostMovieStartup(noBankServices) ||
        noBanks.count != 6 ||
        noBanks.observed[0] != FableFrontendInit2 ||
        noBanks.observed[5] != FableFrontendChangeStateFirstTime)
    {
        return 2;
    }

    SequenceContext failure = {};
    failure.failAt = FableFrontendInitialiseEngine;
    FableFrontendPostMovieServices failureServices = {
        true,
        &RecordStep,
        &failure
    };
    if (
        FableRunFrontendPostMovieStartup(failureServices) ||
        failure.count != 5 ||
        failure.observed[4] != FableFrontendInitialiseEngine)
    {
        return 3;
    }

    printf("FABLETLC_FRONTEND_STARTUP_SEQUENCE PASS\n");
    return 0;
}
