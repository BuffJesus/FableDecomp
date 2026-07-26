// Standalone behaviour test for CIEngine::GetPrimitiveDrawDistanceFactor.
// Provides the global the accessor reads and checks the returned value.
#include <cstdio>

float g_PrimitiveDrawDistanceFactor = 2.5f;

float __fastcall CIEngine__GetPrimitiveDrawDistanceFactor(void *this_ptr)
{
    (void)this_ptr;
    return g_PrimitiveDrawDistanceFactor;
}

int main()
{
    char dummy_obj[8];              // stand-in CIEngine instance (this unused)
    g_PrimitiveDrawDistanceFactor = 3.75f;
    float r = CIEngine__GetPrimitiveDrawDistanceFactor(dummy_obj);
    if (r == 3.75f)
        std::printf("CIENGINE_00b23360_TEST PASS\n");
    else
        std::printf("CIENGINE_00b23360_TEST FAIL got %f\n", r);
    return 0;
}