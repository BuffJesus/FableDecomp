#include <cstdio>

unsigned long __fastcall CSimVoicesDef_GetSizeofClass(const void* self)
{
    (void)self;
    return 0x54;
}

int main()
{
    int dummy = 0;
    unsigned long r = CSimVoicesDef_GetSizeofClass(&dummy);
    if (r == 0x54) {
        std::printf("CSimVoicesDef_00455e8c_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", r);
    return 1;
}