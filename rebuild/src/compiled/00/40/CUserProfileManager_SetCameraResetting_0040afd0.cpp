struct CUserProfileManager {
    char pad0[0xc];
    char flag_c;       // +0xc
    char pad_d;        // +0xd
    char field_e;      // +0xe
    char pad2[0xce - 0xf];
    char field_ce;     // +0xce
};

extern void __fastcall CUPM_Helper1(CUserProfileManager* self);
extern void __fastcall CUPM_Helper2(CUserProfileManager* self);

void __fastcall CUserProfileManager_SetCameraResetting(CUserProfileManager* self, void* edx, char p1, char p2)
{
    if (self->flag_c)
        self->field_ce = p1;
    self->field_e = p1;
    CUPM_Helper1(self);
    if (p2)
        CUPM_Helper2(self);
}