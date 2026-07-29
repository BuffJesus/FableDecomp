#include <cstdio>
#include <cstring>

#include "../../../src/compiled/00/49/CFrontendGameComponent_ChangeSelection_00494380.cpp"

double g_FrontendInitialRepeatDelay_00494380 = 0.5;
double g_FrontendHeldRepeatDelay_00494380 = 0.1;

static double g_Times[4];
static unsigned long g_TimeIndex;
static unsigned long g_TimeCalls;

double __cdecl GetFrontendInputTime_00494380()
{
    ++g_TimeCalls;
    return g_Times[g_TimeIndex++];
}

static double* LastTime(CFrontendGameComponent_00494380* component)
{
    return reinterpret_cast<double*>(
        reinterpret_cast<unsigned char*>(component) + 0x1e48);
}

static bool* IsRepeating(CFrontendGameComponent_00494380* component)
{
    return reinterpret_cast<bool*>(
        reinterpret_cast<unsigned char*>(component) + 0x1e50);
}

static long* LastMovement(CFrontendGameComponent_00494380* component)
{
    return reinterpret_cast<long*>(
        reinterpret_cast<unsigned char*>(component) + 0x1e54);
}

static void SetTimes(double first, double second)
{
    g_Times[0] = first;
    g_Times[1] = second;
    g_TimeIndex = 0;
    g_TimeCalls = 0;
}

int main()
{
    CFrontendGameComponent_00494380 component;
    unsigned char* bytes = reinterpret_cast<unsigned char*>(&component);
    std::memset(bytes, 0x5a, sizeof(component));
    *LastTime(&component) = 10.0;
    *IsRepeating(&component) = true;
    *LastMovement(&component) = 1;

    SetTimes(20.0, 0.0);
    if (!component.ChangeSelection(2) || g_TimeCalls != 1 ||
        *LastTime(&component) != 20.0 || *IsRepeating(&component) ||
        *LastMovement(&component) != 2)
        goto fail;

    SetTimes(20.49, 0.0);
    if (component.ChangeSelection(2) || g_TimeCalls != 1 ||
        *LastTime(&component) != 20.0 || *IsRepeating(&component))
        goto fail;

    SetTimes(20.5, 21.0);
    if (!component.ChangeSelection(2) || g_TimeCalls != 2 ||
        *LastTime(&component) != 21.0 || !*IsRepeating(&component))
        goto fail;

    SetTimes(21.09, 0.0);
    if (component.ChangeSelection(2) || g_TimeCalls != 1)
        goto fail;

    SetTimes(21.1, 22.0);
    if (!component.ChangeSelection(2) || g_TimeCalls != 2 ||
        *LastTime(&component) != 22.0 || !*IsRepeating(&component))
        goto fail;

    for (unsigned long index = 0; index < 0x1e48; ++index)
    {
        if (bytes[index] != 0x5a)
            goto fail;
    }

    std::printf("CFrontendGameComponent_ChangeSelection_00494380_test PASS\n");
    return 0;

fail:
    std::printf("CFrontendGameComponent_ChangeSelection_00494380_test FAIL\n");
    return 1;
}
