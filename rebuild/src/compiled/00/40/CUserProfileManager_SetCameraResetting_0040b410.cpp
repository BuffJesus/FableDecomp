struct CUserProfileManager {
    char pad0[0xc];
    char flag0c;
    char pad_d[0x30-0xd];
    char flag30;
    char pad_31[0xf0-0x31];
    char flagf0;
};

extern void __fastcall CUPM_Notify1(CUserProfileManager* self);
extern void __fastcall CUPM_Notify2(CUserProfileManager* self);

void __fastcall CUserProfileManager_SetCameraResetting(CUserProfileManager* self, void* edx, char p1, char p2)
{
    (void)edx;
    if (self->flag0c)
        self->flagf0 = p1;
    self->flag30 = p1;
    CUPM_Notify1(self);
    if (p2)
        CUPM_Notify2(self);
}