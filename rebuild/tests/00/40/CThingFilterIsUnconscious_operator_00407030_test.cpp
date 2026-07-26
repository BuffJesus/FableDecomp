#include <cstdio>
struct CThingFilter_IsUnconscious { char pad; };
bool __fastcall CThingFilter_IsUnconscious_operatorcall(CThingFilter_IsUnconscious* self, void* thing)
{
    (void)self;
    (void)thing;
    return false;
}
int main()
{
    CThingFilter_IsUnconscious f;
    int dummy = 0;
    if (CThingFilter_IsUnconscious_operatorcall(&f, &dummy) == false) {
        std::printf("CThingFilter_IsUnconscious_00407030_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}