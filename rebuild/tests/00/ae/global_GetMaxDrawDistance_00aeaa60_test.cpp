#include <cstdio>

float g_MaxDrawDistance = 1234.5f;

float __fastcall GetMaxDrawDistance();

int main()
{
    float v = GetMaxDrawDistance();
    if (v == 1234.5f)
        printf("00aeaa60_TEST PASS\n");
    else
        printf("FAIL %f\n", v);
    return 0;
}