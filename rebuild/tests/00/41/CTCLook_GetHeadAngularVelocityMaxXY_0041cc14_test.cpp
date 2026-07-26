// Standalone behaviour test for CTCLook::GetHeadAngularVelocityMaxXY.
// Compile: cl /EHsc test.cpp   (or with the source object linked in).
// Provides the extern global definitions the accessor reads.
#include <stdio.h>

extern unsigned char g_HeadAngVelMaxXY_OverrideFlag;
extern float         g_HeadAngVelMaxXY_OverrideValue;
extern float         g_HeadAngVelMaxXY_Default;

// definitions of the engine globals used as the test fixture
unsigned char g_HeadAngVelMaxXY_OverrideFlag = 0;
float         g_HeadAngVelMaxXY_OverrideValue = 2.5f;
float         g_HeadAngVelMaxXY_Default        = 7.0f;

float __fastcall CTCLook__GetHeadAngularVelocityMaxXY(void *ecx_this)
{
    (void)ecx_this;
    if (g_HeadAngVelMaxXY_OverrideFlag != 0)
        return g_HeadAngVelMaxXY_OverrideValue;
    return g_HeadAngVelMaxXY_Default;
}

static int feq(float a, float b)
{
    float d = a - b;
    if (d < 0) d = -d;
    return d < 0.0001f;
}

int main(void)
{
    void *dummy = (void*)0x1000; // never dereferenced

    // flag == 0 -> returns default
    g_HeadAngVelMaxXY_OverrideFlag = 0;
    float r0 = CTCLook__GetHeadAngularVelocityMaxXY(dummy);

    // flag != 0 -> returns override value
    g_HeadAngVelMaxXY_OverrideFlag = 1;
    float r1 = CTCLook__GetHeadAngularVelocityMaxXY(dummy);

    // any non-zero byte counts
    g_HeadAngVelMaxXY_OverrideFlag = 200;
    float r2 = CTCLook__GetHeadAngularVelocityMaxXY(dummy);

    if (feq(r0, 7.0f) && feq(r1, 2.5f) && feq(r2, 2.5f)) {
        printf("CTCLOOK_0041cc14_TEST PASS\n");
        return 0;
    }
    printf("CTCLOOK_0041cc14_TEST FAIL r0=%f r1=%f r2=%f\n", r0, r1, r2);
    return 1;
}