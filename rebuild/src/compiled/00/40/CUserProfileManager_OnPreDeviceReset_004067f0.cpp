struct CUserProfileManager {
    char pad[0x134];
    int mField;
};

void __fastcall CUserProfileManager_OnPreDeviceReset(CUserProfileManager* self)
{
    self->mField = 0;
}