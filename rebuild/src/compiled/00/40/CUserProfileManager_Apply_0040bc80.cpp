struct CUserProfileManager { char pad[0x33]; char flag; };
extern "C" void __fastcall UPM_Prep(CUserProfileManager* self);
extern "C" void __fastcall UPM_Commit(CUserProfileManager* self);

void __fastcall CUserProfileManager_Apply(CUserProfileManager* self)
{
    UPM_Prep(self);
    if (self->flag)
        UPM_Commit(self);
}