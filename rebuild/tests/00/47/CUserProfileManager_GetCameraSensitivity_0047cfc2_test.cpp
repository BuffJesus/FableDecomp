#include <cstdio>

struct CUserProfileManager {
    char pad0[0xc];
    char flag;
    char pad1[0x24 - 0xd];
    float sensA;
    char pad2[0xe4 - 0x28];
    float sensB;
};

float __fastcall GetCameraSensitivity(CUserProfileManager *self)
{
    if (self->flag != 0)
        return self->sensB;
    return self->sensA;
}

int main()
{
    CUserProfileManager m;
    m.sensA = 1.5f;
    m.sensB = 9.25f;

    m.flag = 0;
    float r0 = GetCameraSensitivity(&m);
    m.flag = 1;
    float r1 = GetCameraSensitivity(&m);

    if (r0 == 1.5f && r1 == 9.25f) {
        printf("OK_0x0047cfc2\n");
        return 0;
    }
    printf("FAIL r0=%f r1=%f\n", r0, r1);
    return 1;
}