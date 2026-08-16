#include <cstdio>
int __fastcall CChunkedFileChunk_GetAsSuperChunk()
{
    return 0;
}
int main()
{
    if (CChunkedFileChunk_GetAsSuperChunk() == 0) { std::printf("AUTO_TINY_00aa7640_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00aa7640_TEST FAIL\n");
    return 1;
}