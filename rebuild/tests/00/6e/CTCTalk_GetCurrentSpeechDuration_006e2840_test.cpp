#include <cstdio>

int g_speechDurationDivisor;

struct CTCTalk
{
    char pad0[0x14];
    int  m_speechTicks;   // +0x14

    float GetCurrentSpeechDuration();
};

float CTCTalk::GetCurrentSpeechDuration()
{
    return (float)m_speechTicks / g_speechDurationDivisor;
}

int main()
{
    g_speechDurationDivisor = 30;
    CTCTalk t;
    t.m_speechTicks = 90;
    float d = t.GetCurrentSpeechDuration();
    // 90 / 30 = 3.0
    if (d > 2.999f && d < 3.001f)
        printf("DURATION_OK 3.0\n");
    else
    {
        printf("DURATION_BAD %f\n", d);
        return 1;
    }

    t.m_speechTicks = 45;
    d = t.GetCurrentSpeechDuration();
    // 45 / 30 = 1.5
    if (d > 1.499f && d < 1.501f)
    {
        printf("DURATION2_OK 1.5\n");
        return 0;
    }
    printf("DURATION2_BAD %f\n", d);
    return 1;
}