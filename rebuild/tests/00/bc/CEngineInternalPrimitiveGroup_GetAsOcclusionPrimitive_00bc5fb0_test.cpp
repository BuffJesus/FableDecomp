#include <cstdio>
void* __fastcall CEngineInternalPrimitiveGroup_GetAsOcclusionPrimitive(void* self)
{
    return self;
}
int main()
{
    int value = 0;
    if (CEngineInternalPrimitiveGroup_GetAsOcclusionPrimitive(&value) == &value) { std::printf("AUTO_TINY_00bc5fb0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00bc5fb0_TEST FAIL\n");
    return 1;
}