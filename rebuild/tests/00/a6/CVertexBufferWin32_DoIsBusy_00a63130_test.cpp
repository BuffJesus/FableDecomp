#include <cstdio>
bool __fastcall CVertexBufferWin32_DoIsBusy()
{
    return false;
}
int main()
{
    if (CVertexBufferWin32_DoIsBusy() == false) { std::printf("AUTO_TINY_00a63130_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00a63130_TEST FAIL\n");
    return 1;
}