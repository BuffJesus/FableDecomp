#pragma optimize("gy",on)

struct CUserProfileManager {
    char pad0[0xc];
    char flag;
    char pad1[0x2c - 0xd];
    int m2c;
    char pad2[0xec - 0x30];
    int mec;
};

extern void __fastcall CUPM_Helper1(CUserProfileManager* self);
extern void __fastcall CUPM_Helper2(CUserProfileManager* self);

void __fastcall CUserProfileManager_SetAntialiasing(CUserProfileManager* self, void* edx, volatile int value, char apply)
{
    (void)edx;
    if (self->flag)
        self->mec = value;
    self->m2c = value;
    CUPM_Helper1(self);
    if (apply)
        CUPM_Helper2(self);
}