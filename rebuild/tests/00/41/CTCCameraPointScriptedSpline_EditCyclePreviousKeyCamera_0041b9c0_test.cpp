#include <stdio.h>
#include <string.h>

struct FableSplineKeyDef_0041B9C0
{
    unsigned int field00;
    unsigned int field04;
    unsigned int field08;
    unsigned int field0C;
    unsigned int field10;
    unsigned int pad14[6];
    unsigned int keyIndex;
};

struct FableCameraPointScriptedSpline_0041B9C0
{
    unsigned int              pad00[2];
    FableSplineKeyDef_0041B9C0* def;
    unsigned int              pad0C[3];
    unsigned int              cachedKey;

    void StepBackward();
    void ApplyKey(unsigned int keyValue, unsigned int* keyRef);
};

static unsigned int  g_stepCalls;
static unsigned int  g_applyCalls;
static void*         g_lastSelf;
static unsigned int  g_lastKeyValue;
static unsigned int* g_lastKeyRef;

void FableCameraPointScriptedSpline_0041B9C0::StepBackward()
{
    ++g_stepCalls;
    g_lastSelf = this;
}

void FableCameraPointScriptedSpline_0041B9C0::ApplyKey(unsigned int keyValue, unsigned int* keyRef)
{
    ++g_applyCalls;
    g_lastSelf = this;
    g_lastKeyValue = keyValue;
    g_lastKeyRef = keyRef;
}

void __fastcall FableCameraSplineEditCyclePreviousKeyCamera_0041B9C0(
    FableCameraPointScriptedSpline_0041B9C0* self)
{
    FableSplineKeyDef_0041B9C0* def = self->def;
    if (def != 0 && self->cachedKey != def->keyIndex)
    {
        self->StepBackward();
        def = self->def;
        self->ApplyKey(def->field10, &def->field04);
        self->cachedKey = self->def->keyIndex;
    }
}

static void Reset()
{
    g_stepCalls = 0;
    g_applyCalls = 0;
    g_lastSelf = 0;
    g_lastKeyValue = 0;
    g_lastKeyRef = 0;
}

static bool CheckNullDef()
{
    Reset();
    FableCameraPointScriptedSpline_0041B9C0 spline;
    memset(&spline, 0, sizeof(spline));
    spline.def = 0;
    spline.cachedKey = 0x11223344;
    FableCameraSplineEditCyclePreviousKeyCamera_0041B9C0(&spline);
    return g_stepCalls == 0 && g_applyCalls == 0 && spline.cachedKey == 0x11223344;
}

static bool CheckAlreadyCached()
{
    Reset();
    FableSplineKeyDef_0041B9C0 def;
    memset(&def, 0, sizeof(def));
    def.keyIndex = 0x40;
    FableCameraPointScriptedSpline_0041B9C0 spline;
    memset(&spline, 0, sizeof(spline));
    spline.def = &def;
    spline.cachedKey = 0x40;
    FableCameraSplineEditCyclePreviousKeyCamera_0041B9C0(&spline);
    return g_stepCalls == 0 && g_applyCalls == 0 && spline.cachedKey == 0x40;
}

static bool CheckCyclePrevious()
{
    Reset();
    FableSplineKeyDef_0041B9C0 def;
    memset(&def, 0, sizeof(def));
    def.field04 = 0xAAAA0004;
    def.field10 = 0xBBBB0010;
    def.keyIndex = 0x7;
    FableCameraPointScriptedSpline_0041B9C0 spline;
    memset(&spline, 0, sizeof(spline));
    spline.def = &def;
    spline.cachedKey = 0x3;
    FableCameraSplineEditCyclePreviousKeyCamera_0041B9C0(&spline);
    return
        g_stepCalls == 1 &&
        g_applyCalls == 1 &&
        g_lastSelf == &spline &&
        g_lastKeyValue == 0xBBBB0010 &&
        g_lastKeyRef == &def.field04 &&
        spline.cachedKey == 0x7;
}

int main()
{
    bool passed = CheckNullDef() && CheckAlreadyCached() && CheckCyclePrevious();
    if (!passed)
    {
        printf("FABLETLC_CAMERA_SPLINE_CYCLE_PREV_B9C0 FAIL\n");
        return 1;
    }
    printf("FABLETLC_CAMERA_SPLINE_CYCLE_PREV_B9C0 PASS\n");
    return 0;
}