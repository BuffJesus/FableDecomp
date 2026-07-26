// Standalone behaviour test for CTCPerceiveThing::IsUpdatingThisFrame @ 0x007443f0
#include <stdio.h>

extern int __cdecl GetFrameCount(void);

struct CTCPerceiveThing
{
    char  _pad0[0x38];
    int   m_updateFrame;   // +0x38
};

bool __fastcall CTCPerceiveThing_IsUpdatingThisFrame(CTCPerceiveThing *self)
{
    return (GetFrameCount() % 4) == self->m_updateFrame;
}

// Controllable stub for the extern frame counter.
static int g_frame = 0;
int __cdecl GetFrameCount(void) { return g_frame; }

int main(void)
{
    CTCPerceiveThing obj;
    obj.m_updateFrame = 2;

    // frame 6 -> 6 % 4 == 2 -> should match field (2) -> true
    g_frame = 6;
    bool r1 = CTCPerceiveThing_IsUpdatingThisFrame(&obj);

    // frame 7 -> 7 % 4 == 3 -> != 2 -> false
    g_frame = 7;
    bool r2 = CTCPerceiveThing_IsUpdatingThisFrame(&obj);

    // frame 2 -> 2 % 4 == 2 -> == 2 -> true
    g_frame = 2;
    bool r3 = CTCPerceiveThing_IsUpdatingThisFrame(&obj);

    // negative frame -2 -> -2 % 4 == -2 (signed) -> field -2 matches
    obj.m_updateFrame = -2;
    g_frame = -2;
    bool r4 = CTCPerceiveThing_IsUpdatingThisFrame(&obj);

    if (r1 && !r2 && r3 && r4)
    {
        printf("CTCPERCEIVETHING_007443f0_TEST PASS\n");
        return 0;
    }
    printf("CTCPERCEIVETHING_007443f0_TEST FAIL r1=%d r2=%d r3=%d r4=%d\n",
           (int)r1, (int)r2, (int)r3, (int)r4);
    return 1;
}