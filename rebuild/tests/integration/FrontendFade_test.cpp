#include "fable_frontend_fade.h"
#include <stdio.h>

int main()
{
    const float durations[] = { 2.0f, 8.0f };
    for (unsigned d = 0; d != 2; ++d)
        for (unsigned incoming = 0; incoming != 2; ++incoming)
            for (unsigned step = 0; step <= 64; ++step)
            {
                const float elapsed = durations[d] * step / 64.0f;
                printf("%u %u %u %u\n", d, incoming, step,
                    FableFrontendFadeAlpha(incoming != 0, elapsed, durations[d]));
            }
    return 0;
}
