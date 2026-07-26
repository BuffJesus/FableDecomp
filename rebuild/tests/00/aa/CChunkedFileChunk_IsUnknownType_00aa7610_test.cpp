#include <cstdio>
int __fastcall CChunkedFileChunk_IsUnknownType()
{
    return 0;
}
int main()
{
    if (CChunkedFileChunk_IsUnknownType() == 0) { std::printf("AUTO_TINY_00aa7610_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00aa7610_TEST FAIL\n");
    return 1;
}