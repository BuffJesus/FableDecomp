#include <cstdio>

struct Result
{
    int providerCalls;
    int resetCalls;
    int releaseCalls;
    int layerReleases;
    int finalCount;
};

static Result Run(bool hasTarget, bool targetBlocked, int count)
{
    Result result = {0, 0, 0, 0, count};
    if (hasTarget && !targetBlocked)
    {
        ++result.providerCalls;
        if (result.finalCount >= 0)
        {
            ++result.resetCalls;
            do
            {
                ++result.layerReleases;
                --result.finalCount;
            }
            while (result.finalCount != -1);
            result.finalCount = -1;
        }
        else
        {
            ++result.releaseCalls;
            result.finalCount = -1;
        }
    }
    return result;
}

int main()
{
    Result r = Run(false, false, 3);
    if (r.providerCalls != 0 || r.finalCount != 3) return 1;
    r = Run(true, true, 2);
    if (r.providerCalls != 0 || r.finalCount != 2) return 2;
    r = Run(true, false, -1);
    if (r.providerCalls != 1 || r.releaseCalls != 1 || r.resetCalls != 0 || r.finalCount != -1) return 3;
    r = Run(true, false, 0);
    if (r.resetCalls != 1 || r.layerReleases != 1 || r.releaseCalls != 0 || r.finalCount != -1) return 4;
    r = Run(true, false, 3);
    if (r.resetCalls != 1 || r.layerReleases != 4 || r.finalCount != -1) return 5;
    std::puts("CAMERA_DEFAULT_PASS");
    return 0;
}