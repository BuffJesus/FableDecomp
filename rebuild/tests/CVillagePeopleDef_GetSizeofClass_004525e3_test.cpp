#include <cstdio>

struct CVillagePeopleDef { char pad[0xC4]; };

unsigned long __fastcall CVillagePeopleDef_GetSizeofClass(const CVillagePeopleDef* self)
{
    (void)self;
    return 0xC4;
}

int main()
{
    CVillagePeopleDef d;
    unsigned long r = CVillagePeopleDef_GetSizeofClass(&d);
    if (r != 0xC4) { std::printf("FAIL got %lu\n", r); return 1; }
    std::printf("CVillagePeopleDef_004525e3_TEST PASS\n");
    return 0;
}