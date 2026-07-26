struct CUserProfileManager {
    char pad0[0xc];
    char flag0c;
    char pad1[0x13 - 0xd];
    char b13;
    char pad2[0xd3 - 0x14];
    char bd3;
};

extern void __fastcall UPM_Update(CUserProfileManager* self);
extern void __fastcall UPM_Refresh(CUserProfileManager* self);

void __fastcall CUserProfileManager_SetCameraResetting(CUserProfileManager* self, int edx_pad, char p1, char p2)
{
    (void)edx_pad;
    if (self->flag0c)
        self->bd3 = p1;
    self->b13 = p1;
    UPM_Update(self);
    if (p2)
        UPM_Refresh(self);
}