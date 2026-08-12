// CTCWillLightning::SetTimeToCastLightning @ 0x0079f5f0
// void __fastcall (CTCWillLightning*, float)
//   [ecx+0x5b] = 1;
//   [ecx+0x1c] = (int)( (float)g_int * param );

int g_lightningCastTicks; // global int at 0x1375550, loaded via fild

struct CTCWillLightning {
    char pad00[0x1c];
    int   m_timeToCast;   // +0x1c
    char pad20[0x5b - 0x20];
    char m_castFlag;      // +0x5b

    void SetTimeToCastLightning(float scale);
};

void CTCWillLightning::SetTimeToCastLightning(float scale)
{
    this->m_castFlag = 1;
    this->m_timeToCast = (int)((float)g_lightningCastTicks * scale);
}