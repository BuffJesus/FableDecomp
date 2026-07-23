struct CUserProfileManager {
    char pad0[0xc];
    char flagC;       // +0x0c
    char pad1[0x0b];
    char f18;         // +0x18
    char pad2[0xbf];
    char fD8;         // +0xd8
};

extern void __fastcall CUserProfileManager_OnResetChanged(CUserProfileManager* self);
extern void __fastcall CUserProfileManager_OnSecondFlag(CUserProfileManager* self);

void __fastcall CUserProfileManager_SetCameraResetting(CUserProfileManager* self, void* edx, char a, char b)
{
    if (self->flagC)
        self->fD8 = a;
    self->f18 = a;
    CUserProfileManager_OnResetChanged(self);
    if (b)
        CUserProfileManager_OnSecondFlag(self);
}