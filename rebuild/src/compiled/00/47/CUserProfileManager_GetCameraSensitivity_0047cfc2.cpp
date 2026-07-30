#pragma optimize("s",on)
struct CUserProfileManager {
    char pad0[0xc];
    char flag;          // +0xc
    char pad1[0x24 - 0xd];
    float sensA;        // +0x24
    char pad2[0xe4 - 0x28];
    float sensB;        // +0xe4
};

float __fastcall GetCameraSensitivity(CUserProfileManager *self)
{
    if (self->flag != 0)
        return self->sensB;
    return self->sensA;
}