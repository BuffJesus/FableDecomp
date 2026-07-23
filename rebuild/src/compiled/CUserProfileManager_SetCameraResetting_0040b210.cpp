struct CUserProfileManager {
    char pad0[0xc];
    char m_flagC;      // +0xc
    char pad1[0xa];
    char m_flag17;     // +0x17
    char pad2[0xbf];
    char m_flagD7;     // +0xd7
};

void __fastcall CUserProfileManager_Helper1(CUserProfileManager* self);
void __fastcall CUserProfileManager_Helper2(CUserProfileManager* self);

void __fastcall CUserProfileManager_SetCameraResetting(CUserProfileManager* self, int edx_dummy, char p1, char p2)
{
    if (self->m_flagC)
        self->m_flagD7 = p1;
    self->m_flag17 = p1;
    CUserProfileManager_Helper1(self);
    if (p2)
        CUserProfileManager_Helper2(self);
}