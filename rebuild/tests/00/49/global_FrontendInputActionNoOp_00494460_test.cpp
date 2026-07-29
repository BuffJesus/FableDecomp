#include <cstdio>

#include "../../../src/compiled/00/49/global_FrontendInputActionNoOp_00494460.cpp"

int main()
{
    volatile unsigned long sentinel = 0x87654321;

    FrontendInputActionNoOp_00494460();
    FrontendInputActionNoOp_00494460();

    if (sentinel != 0x87654321)
    {
        std::printf("global_FrontendInputActionNoOp_00494460_test FAIL\n");
        return 1;
    }

    std::printf("global_FrontendInputActionNoOp_00494460_test PASS\n");
    return 0;
}
