#include <cstdio>
int __fastcall CEnemyDef_GetSizeofClass()
{
    return 44;
}
int main()
{
    if (CEnemyDef_GetSizeofClass() == 44) { std::printf("AUTO_TINY_004d3ddb_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_004d3ddb_TEST FAIL\n");
    return 1;
}