#include <cstdio>

struct CIEngine { };

bool __fastcall CIEngine_IsPreloaded(const CIEngine* self)
{
    (void)self;
    return true;
}

int main()
{
    CIEngine e;
    if (CIEngine_IsPreloaded(&e) == true) {
        std::printf("CIEngine_00b235d0_TEST PASS\n");
        return 0;
    }
    std::printf("CIEngine_00b235d0_TEST FAIL\n");
    return 1;
}