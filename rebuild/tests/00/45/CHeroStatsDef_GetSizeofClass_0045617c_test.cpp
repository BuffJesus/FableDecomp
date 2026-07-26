#include <cstdio>

struct CHeroStatsDef {};

unsigned long __fastcall CHeroStatsDef_GetSizeofClass(CHeroStatsDef* self)
{
    (void)self;
    return 0x138;
}

int main()
{
    CHeroStatsDef obj;
    unsigned long r = CHeroStatsDef_GetSizeofClass(&obj);
    if (r == 0x138) {
        std::printf("CHeroStatsDef_0045617c_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", r);
    return 1;
}