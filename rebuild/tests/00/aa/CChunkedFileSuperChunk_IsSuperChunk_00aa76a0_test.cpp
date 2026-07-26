#include <cstdio>
int __fastcall CChunkedFileSuperChunk_IsSuperChunk()
{
    return 1;
}
int main()
{
    if (CChunkedFileSuperChunk_IsSuperChunk() == 1) { std::printf("AUTO_TINY_00aa76a0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00aa76a0_TEST FAIL\n");
    return 1;
}