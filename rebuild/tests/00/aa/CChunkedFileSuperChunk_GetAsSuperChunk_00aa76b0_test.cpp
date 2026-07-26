#include <cstdio>
void* __fastcall CChunkedFileSuperChunk_GetAsSuperChunk(void* self)
{
    return self;
}
int main()
{
    int value = 0;
    if (CChunkedFileSuperChunk_GetAsSuperChunk(&value) == &value) { std::printf("AUTO_TINY_00aa76b0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00aa76b0_TEST FAIL\n");
    return 1;
}