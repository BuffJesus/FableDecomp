#include <cstdio>

struct CThing {
    char pad0[0x28];
    unsigned char f28;
    char pad1[0x5c-0x29];
    void* f5c;
};

bool __fastcall GetTCDriver_CTCDExperienceOrb(CThing* self, int edx, void** out);

int main()
{
    CThing t;
    for (int i = 0; i < (int)sizeof(t); ++i) ((char*)&t)[i] = 0;
    int mk = 12345;
    void* pmk = &mk;
    t.f5c = pmk;

    void* out = 0;

    // bit 2 clear -> false, out untouched
    t.f28 = 0;
    if (GetTCDriver_CTCDExperienceOrb(&t, 0, &out)) { printf("FAIL1\n"); return 0; }
    if (out != 0) { printf("FAIL2\n"); return 0; }

    // bit 2 set -> true, out = f5c
    t.f28 = 2;
    if (!GetTCDriver_CTCDExperienceOrb(&t, 0, &out)) { printf("FAIL3\n"); return 0; }
    if (out != pmk) { printf("FAIL4\n"); return 0; }

    // other bits set but not bit 2 -> false
    t.f28 = (unsigned char)~2;
    out = 0;
    if (GetTCDriver_CTCDExperienceOrb(&t, 0, &out)) { printf("FAIL5\n"); return 0; }

    printf("007512c0_TEST PASS\n");
    return 0;
}