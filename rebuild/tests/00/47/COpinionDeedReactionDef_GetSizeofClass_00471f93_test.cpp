#include <cstdio>
int __fastcall COpinionDeedReactionDef_GetSizeofClass()
{
    return 80;
}
int main()
{
    if (COpinionDeedReactionDef_GetSizeofClass() == 80) { std::printf("AUTO_TINY_00471f93_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00471f93_TEST FAIL\n");
    return 1;
}