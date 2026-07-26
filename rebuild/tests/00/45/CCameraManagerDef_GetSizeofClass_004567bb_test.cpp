#include <cstdio>
struct CCameraManagerDef {};
unsigned long __fastcall CCameraManagerDef_GetSizeofClass(const CCameraManagerDef* self)
{
    (void)self;
    return 0x74;
}
int main()
{
    CCameraManagerDef d;
    unsigned long r = CCameraManagerDef_GetSizeofClass(&d);
    if (r == 0x74) {
        std::printf("CCameraManagerDef_004567bb_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", r);
    return 1;
}