struct CUserProfileManager {
    char pad00[0x0c];
    unsigned char m_flag_0c;
    char pad0d[0x32 - 0x0d];
    unsigned char m_byte_32;
    char pad33[0xf1 - 0x33];
    unsigned char m_byte_f1;
};

extern void __fastcall UPM_MethodA(CUserProfileManager* self);
extern void __fastcall UPM_MethodB(CUserProfileManager* self);

void __fastcall CUserProfileManager_SetCameraResetting(CUserProfileManager* self, int edx, char p1, char p2)
{
    (void)edx;
    if (self->m_flag_0c)
        self->m_byte_f1 = (unsigned char)p1;
    self->m_byte_32 = (unsigned char)p1;
    UPM_MethodA(self);
    if (p2)
        UPM_MethodB(self);
}