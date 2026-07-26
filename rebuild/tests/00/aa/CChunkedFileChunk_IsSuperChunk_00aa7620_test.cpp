#include <cstdio>
int __fastcall CChunkedFileChunk_IsSuperChunk()
{
    return 0;
}
int main()
{
    if (CChunkedFileChunk_IsSuperChunk() == 0) { std::printf("AUTO_TINY_00aa7620_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00aa7620_TEST FAIL\n");
    return 1;
}