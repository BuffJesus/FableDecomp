#include <stdio.h>

static const float g_animScale = 65535.0f / 360.0f;

struct C3DAnimationSequence
{
    int   field0;
    int   field4;
    short m_value;

    void InitialiseFromFileData(float f)
    {
        m_value = (short)(f * g_animScale);
    }
};

void InitialiseFromFileData_C3DAnimationSequence(C3DAnimationSequence* self, float f);

int main()
{
    C3DAnimationSequence a;
    a.field0 = 0; a.field4 = 0; a.m_value = 0;

    // 360 deg * (65535/360) = 65535 -> truncates to short = -1 (0xFFFF)
    InitialiseFromFileData_C3DAnimationSequence(&a, 1.0f);
    short expect1 = (short)(1.0f * g_animScale);
    if (a.m_value != expect1) { printf("FAIL v1 %d\n", a.m_value); return 1; }

    InitialiseFromFileData_C3DAnimationSequence(&a, 90.0f);
    short expect2 = (short)(90.0f * g_animScale);
    if (a.m_value != expect2) { printf("FAIL v2 %d\n", a.m_value); return 1; }

    InitialiseFromFileData_C3DAnimationSequence(&a, 0.0f);
    if (a.m_value != 0) { printf("FAIL v3 %d\n", a.m_value); return 1; }

    printf("INITFROMFILEDATA_OK %d %d\n", (int)expect1, (int)expect2);
    return 0;
}