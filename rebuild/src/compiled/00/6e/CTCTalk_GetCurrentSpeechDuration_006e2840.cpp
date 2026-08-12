// CTCTalk::GetCurrentSpeechDuration @ 006e2840
// fild [ecx+0x14] ; fidiv [g_divisor] ; ret
// => (float)member / (int global) -> fidiv keeps divisor as int

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