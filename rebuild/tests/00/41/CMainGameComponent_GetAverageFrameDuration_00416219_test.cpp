#include <stdio.h>
#include "fable_game.h"

static int g_calls;
static const void* g_receiver;
static double g_value;

double CFrameRateSmoother::GetSmoothedTime() const
{
    ++g_calls;
    g_receiver = this;
    return g_value;
}

// GetAverageFrameDuration is protected; a derived view calls it.
struct AverageFrameDurationProbe : CMainGameComponent
{
    float Call() const { return GetAverageFrameDuration(); }
};

int main()
{
    static union
    {
        double align;
        unsigned char bytes[sizeof(CMainGameComponent)];
    } storage;
    const AverageFrameDurationProbe* component =
        reinterpret_cast<const AverageFrameDurationProbe*>(storage.bytes);
    const double values[] = { 0.1234567891234, -2.5, 1.0e40, 0.0 };
    for (int i = 0; i < 4; ++i)
    {
        g_value = values[i];
        volatile float result = component->Call();
        if (g_calls != i + 1) return 1;
        // The smoother is the member at +0x16178.
        if (g_receiver != storage.bytes + 0x16178) return 2;
        // Narrowed to float on return (1e40 overflows to +inf).
        volatile float expected = static_cast<float>(values[i]);
        if (result != expected) return 3 + i;
    }
    puts("FABLETLC_AVERAGE_FRAME_DURATION PASS");
    return 0;
}
