#include <cstdio>

extern float g_maxDrawDistance;
float __fastcall GetMaxDrawDistance();

int main()
{
    g_maxDrawDistance = 1234.5f;
    float v = GetMaxDrawDistance();
    if (v == 1234.5f)
        printf("00aeaa00_TEST PASS\n");
    else
        printf("FAIL %f\n", v);
    return 0;
}