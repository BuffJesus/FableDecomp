#include <cstdio>

#include "../../../src/compiled/00/49/CFrontendGameComponent_RejectInputAction_00494490.cpp"

int main()
{
    CFrontendGameComponent_00494490 component;

    if (component.RejectInputAction(-1) ||
        component.RejectInputAction(0) ||
        component.RejectInputAction(0x7fffffff))
    {
        std::printf(
            "CFrontendGameComponent_RejectInputAction_00494490_test FAIL\n");
        return 1;
    }

    std::printf(
        "CFrontendGameComponent_RejectInputAction_00494490_test PASS\n");
    return 0;
}
