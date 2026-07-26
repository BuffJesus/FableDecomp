#include <cstdio>

struct CLandscapeBackgroundPatch { char _pad[4]; };

bool __fastcall CLandscapeBackgroundPatch_IsPatch(CLandscapeBackgroundPatch* self)
{
    return true;
}

int main()
{
    CLandscapeBackgroundPatch obj;
    if (CLandscapeBackgroundPatch_IsPatch(&obj) == true) {
        std::printf("CLandscapeBackgroundPatch_00be8dc0_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}