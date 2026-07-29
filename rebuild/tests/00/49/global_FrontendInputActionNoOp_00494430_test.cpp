#include <cstdio>

#include "../../../src/compiled/00/49/global_FrontendInputActionNoOp_00494430.cpp"

int main()
{
    volatile unsigned long sentinel = 0x12345678;

    FrontendInputActionNoOp_00494430();
    FrontendInputActionNoOp_00494430();

    if (sentinel != 0x12345678)
    {
        std::printf("global_FrontendInputActionNoOp_00494430_test FAIL\n");
        return 1;
    }

    std::printf("global_FrontendInputActionNoOp_00494430_test PASS\n");
    return 0;
}
