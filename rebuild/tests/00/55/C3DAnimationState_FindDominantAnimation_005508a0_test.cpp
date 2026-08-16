#include <cstdio>
void* __fastcall C3DAnimationState_FindDominantAnimation(void* self)
{
    return self;
}
int main()
{
    int value = 0;
    if (C3DAnimationState_FindDominantAnimation(&value) == &value) { std::printf("AUTO_TINY_005508a0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_005508a0_TEST FAIL\n");
    return 1;
}