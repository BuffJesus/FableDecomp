#include <cstdio>
bool __fastcall CCreatureAction_JackScream_IsToEvadeAllHits()
{
    return true;
}
int main()
{
    if (CCreatureAction_JackScream_IsToEvadeAllHits() == true) { std::printf("AUTO_TINY_00977190_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00977190_TEST FAIL\n");
    return 1;
}