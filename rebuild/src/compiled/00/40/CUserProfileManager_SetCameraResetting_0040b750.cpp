struct CUserProfileManager {
    char pad0[0xc];
    char flagC;
    char pad1[0x50-0x0d];
    char resetFlag50;
    char pad2[0x10c-0x51];
    char field10c;
};

extern void __fastcall CUserProfileManager_Apply(CUserProfileManager* self);
extern void __fastcall CUserProfileManager_Notify(CUserProfileManager* self);

void __fastcall CUserProfileManager_SetCameraResetting(CUserProfileManager* self, void* /*edx*/, char resetting, char notify)
{
    if (self->flagC)
        self->field10c = resetting;
    self->resetFlag50 = resetting;
    CUserProfileManager_Apply(self);
    if (notify)
        CUserProfileManager_Notify(self);
}