struct Sub { char pad[0x80]; float m_Field80; };
struct Root { char pad[0xDC]; Sub* m_Sub; };
extern "C" Root* __fastcall GetHeroStatsRoot(void);
extern float g_threshold;
extern float g_const2;
extern int   g_count;
struct CTCHeroStats;
long __fastcall GetFatnessReducingFrequencyInFrames(CTCHeroStats*)
{
    Root* root = GetHeroStatsRoot();
    float x = root->m_Sub->m_Field80;
    if (x > g_threshold)
    {
        float v = x * (float)g_count * g_const2;
        float one = 1.0f;
        long n = (long)(v + 0.5f);
        float d = (float)n - one;
        if (*(long*)&v == *(long*)&d)
            --n;
        return n;
    }
    return g_count * 0x988;
}
