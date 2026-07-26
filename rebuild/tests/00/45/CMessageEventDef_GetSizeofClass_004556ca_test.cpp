#include <cstdio>

unsigned long __fastcall CMessageEventDef_GetSizeofClass(void* self)
{
    (void)self;
    return 0x50;
}

int main()
{
    unsigned long r = CMessageEventDef_GetSizeofClass(0);
    if (r == 0x50) {
        std::printf("CMessageEventDef_004556ca_TEST PASS\n");
        return 0;
    }
    std::printf("CMessageEventDef_004556ca_TEST FAIL got=%lu\n", r);
    return 1;
}