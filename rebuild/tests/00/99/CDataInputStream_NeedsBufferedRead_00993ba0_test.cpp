#include <cstdio>
bool __fastcall CDataInputStream_NeedsBufferedRead(void*, int, int)
{
    return false;
}
int main()
{
    int object = 0;
    if (!CDataInputStream_NeedsBufferedRead(&object, 1, 2)) { std::printf("AUTO_TINY_00993ba0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00993ba0_TEST FAIL\n");
    return 1;
}