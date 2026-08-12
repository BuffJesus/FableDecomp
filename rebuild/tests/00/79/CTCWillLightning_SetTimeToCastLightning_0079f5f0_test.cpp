#include <cstdio>

int g_lightningCastTicks;

struct CTCWillLightning {
    char pad00[0x1c];
    int   m_timeToCast;
    char pad20[0x5b - 0x20];
    char m_castFlag;

    void SetTimeToCastLightning(float scale)
    {
        this->m_castFlag = 1;
        this->m_timeToCast = (int)((float)g_lightningCastTicks * scale);
    }
};

int main()
{
    g_lightningCastTicks = 200;
    CTCWillLightning obj;
    obj.m_castFlag = 0;
    obj.m_timeToCast = -1;

    obj.SetTimeToCastLightning(2.5f); // 200*2.5 = 500
    if (obj.m_castFlag != 1) { printf("FAIL flag\n"); return 1; }
    if (obj.m_timeToCast != 500) { printf("FAIL time=%d\n", obj.m_timeToCast); return 1; }

    g_lightningCastTicks = 30;
    obj.SetTimeToCastLightning(1.9f); // 30*1.9 = 56.999 -> 56
    if (obj.m_timeToCast != 56) { printf("FAIL time2=%d\n", obj.m_timeToCast); return 1; }

    printf("LIGHTNING_OK\n");
    return 0;
}