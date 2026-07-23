struct CUserProfileManager {
    char pad0[0xc];
    char m_flag0c;
    char pad_d[0x11-0xc-1];
    char m_val11;
    char pad_11[0xd1-0x11-1];
    char m_valD1;
};

extern void __fastcall CUserProfileManager_Sub1(CUserProfileManager* self);
extern void __fastcall CUserProfileManager_Sub2(CUserProfileManager* self);

void __fastcall CUserProfileManager_SetCameraResetting(CUserProfileManager* self, void* edx, char a, char b)
{
    if (self->m_flag0c)
        self->m_valD1 = a;
    self->m_val11 = a;
    CUserProfileManager_Sub1(self);
    if (b)
        CUserProfileManager_Sub2(self);
}