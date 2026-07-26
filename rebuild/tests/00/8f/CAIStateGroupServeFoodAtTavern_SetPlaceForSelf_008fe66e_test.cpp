#include <cstdio>
bool __fastcall CAIStateGroup_ServeFoodAtTavern_SetPlaceForSelf()
{
    return false;
}
int main()
{
    if (CAIStateGroup_ServeFoodAtTavern_SetPlaceForSelf() == false) { std::printf("AUTO_TINY_008fe66e_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_008fe66e_TEST FAIL\n");
    return 1;
}