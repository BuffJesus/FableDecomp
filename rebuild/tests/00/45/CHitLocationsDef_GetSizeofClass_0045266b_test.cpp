#include <cstdio>
struct CHitLocationsDef {};
unsigned long __fastcall CHitLocationsDef_GetSizeofClass(const CHitLocationsDef* self)
{
    (void)self;
    return 0x34;
}
int main()
{
    CHitLocationsDef obj;
    unsigned long r = CHitLocationsDef_GetSizeofClass(&obj);
    if (r == 0x34) { std::printf("CHitLocationsDef_0045266b_TEST PASS\n"); return 0; }
    std::printf("FAIL got %lu\n", r); return 1;
}