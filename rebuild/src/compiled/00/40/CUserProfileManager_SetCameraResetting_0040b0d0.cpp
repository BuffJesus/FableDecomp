struct CUserProfileManager {
    char pad0[0xc];
    char flag_c;       // +0x0c
    char pad_d[0x12-0x0d];
    char field_12;     // +0x12
    char pad2[0xd2-0x13];
    char field_d2;     // +0xd2
};

void __fastcall CUserProfileManager_ResetSub(CUserProfileManager* self);
void __fastcall CUserProfileManager_ResetSub2(CUserProfileManager* self);

void __fastcall CUserProfileManager_SetCameraResetting(CUserProfileManager* self, int edx_, char p1, char p2)
{
    if (self->flag_c)
        self->field_d2 = p1;
    self->field_12 = p1;
    CUserProfileManager_ResetSub(self);
    if (p2)
        CUserProfileManager_ResetSub2(self);
}