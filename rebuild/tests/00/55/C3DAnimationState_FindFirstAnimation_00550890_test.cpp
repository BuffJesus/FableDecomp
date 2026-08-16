#include <cstdio>
void* __fastcall C3DAnimationState_FindFirstAnimation(void* self)
{
    return self;
}
int main()
{
    int value = 0;
    if (C3DAnimationState_FindFirstAnimation(&value) == &value) { std::printf("AUTO_TINY_00550890_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00550890_TEST FAIL\n");
    return 1;
}