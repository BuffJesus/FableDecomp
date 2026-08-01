#include <stdio.h>
#include <math.h>

extern const float CPlayerGui_UpdateTimer_00437500_Upper;
extern const float CPlayerGui_UpdateTimer_00437500_Lower;

void __stdcall CPlayerGui_UpdateTimer_00437500(
    const float* pRate,
    float stepUp,
    float stepDown,
    int* pMode,
    float* pTimer)
{
    switch (*pMode) {
    case 2:
        *pTimer = *pTimer + *pRate / stepUp;
        break;
    case 3:
        *pTimer = *pTimer - *pRate / stepDown;
        break;
    }

    if (*pTimer <= CPlayerGui_UpdateTimer_00437500_Upper) {
        *pMode = 1;
        *pTimer = 0.0f;
    } else if (*pTimer >= CPlayerGui_UpdateTimer_00437500_Lower) {
        *pMode = 0;
        *pTimer = 1.0f;
    }
}

const float CPlayerGui_UpdateTimer_00437500_Upper = 1.0f;
const float CPlayerGui_UpdateTimer_00437500_Lower = 0.0f;

static int NearlyEqual(float a, float b)
{
    return (float)fabs(a - b) < 0.00001f;
}

static int Check(int cond, const char* msg)
{
    if (!cond) { printf("FAIL: %s\n", msg); return 0; }
    return 1;
}

int main()
{
    int ok = 1;

    /* Case 2 (add): timer stays <= upper -> clamp low branch (mode=1, timer=0). */
    {
        float rate = 4.0f;
        int mode = 2;
        float timer = 0.25f; /* 0.25 + 4/8 = 0.75 <= 1.0 */
        CPlayerGui_UpdateTimer_00437500(&rate, 8.0f, 2.0f, &mode, &timer);
        ok &= Check(mode == 1 && NearlyEqual(timer, 0.0f),
                    "case2 add stays<=upper -> mode1/0");
    }

    /* Case 2 (add): timer exceeds upper and is >=lower -> mode=0, timer=1.0. */
    {
        float rate = 4.0f;
        int mode = 2;
        float timer = 0.9f; /* 0.9 + 4/2 = 2.9 > 1.0, >= 0.0 */
        CPlayerGui_UpdateTimer_00437500(&rate, 2.0f, 2.0f, &mode, &timer);
        ok &= Check(mode == 0 && NearlyEqual(timer, 1.0f),
                    "case2 add above upper -> mode0/1.0");
    }

    /* Case 3 (subr): timer drops below upper -> mode=1, timer=0. */
    {
        float rate = 4.0f;
        int mode = 3;
        float timer = 0.25f; /* 0.25 - 4/4 = -0.75 <= 1.0 */
        CPlayerGui_UpdateTimer_00437500(&rate, 8.0f, 4.0f, &mode, &timer);
        ok &= Check(mode == 1 && NearlyEqual(timer, 0.0f),
                    "case3 subr low -> mode1/0");
    }

    /* Case 3 (subr): timer stays above upper and >=lower -> mode=0, timer=1.0. */
    {
        float rate = 1.0f;
        int mode = 3;
        float timer = 2.0f; /* 2.0 - 1/8 = 1.875 > 1.0, >= 0.0 */
        CPlayerGui_UpdateTimer_00437500(&rate, 8.0f, 8.0f, &mode, &timer);
        ok &= Check(mode == 0 && NearlyEqual(timer, 1.0f),
                    "case3 subr high -> mode0/1.0");
    }

    /* Neutral mode (not 2/3): no accumulation; timer 0.5 <= 1.0 -> mode=1, timer=0. */
    {
        float rate = 4.0f;
        int mode = 5;
        float timer = 0.5f;
        CPlayerGui_UpdateTimer_00437500(&rate, 8.0f, 4.0f, &mode, &timer);
        ok &= Check(mode == 1 && NearlyEqual(timer, 0.0f),
                    "neutral mode clamps<=upper -> mode1/0");
    }

    if (!ok)
        return 1;

    puts("PASS CPlayerGui::UpdateTimer 0x00437500");
    return 0;
}