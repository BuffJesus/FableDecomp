#include <stdio.h>

struct CThing {
    char pad00[0x2c];
    unsigned char f2c;
    char pad2d[0x5c-0x2d];
    void* f5c;
};

bool __fastcall GetTCDriver_CTCDExperienceOrb(CThing* self, int, void** out);

int main()
{
    CThing t;
    for (int i = 0; i < (int)sizeof(t); ++i) ((char*)&t)[i] = 0;
    int marker = 12345;
    void* pmarker = (void*)(&marker);
    t.f5c = pmarker;

    void* out = (void*)0xDEAD;

    // flag clear -> false, out untouched
    t.f2c = 0x00;
    void** pout = (void**)(&out);
    bool r1 = GetTCDriver_CTCDExperienceOrb(&t, 0, pout);

    // flag set -> true, out = f5c
    t.f2c = 0x10;
    void* out2 = 0;
    void** pout2 = (void**)(&out2);
    bool r2 = GetTCDriver_CTCDExperienceOrb(&t, 0, pout2);

    // other bits set but not 0x10 -> false
    t.f2c = 0xEF;
    void* out3 = 0;
    void** pout3 = (void**)(&out3);
    bool r3 = GetTCDriver_CTCDExperienceOrb(&t, 0, pout3);

    if (r1 == false && out == (void*)0xDEAD &&
        r2 == true  && out2 == pmarker &&
        r3 == false && out3 == 0) {
        printf("00617d30_TEST PASS\n");
    } else {
        printf("FAIL r1=%d r2=%d r3=%d\n", (int)r1, (int)r2, (int)r3);
    }
    return 0;
}