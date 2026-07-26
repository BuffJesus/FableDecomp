#include <cstdio>
int __fastcall CAnimComponentSetCreatureMode_GetType()
{
    return 10;
}
int main()
{
    if (CAnimComponentSetCreatureMode_GetType() == 10) { std::printf("AUTO_TINY_005df830_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_005df830_TEST FAIL\n");
    return 1;
}