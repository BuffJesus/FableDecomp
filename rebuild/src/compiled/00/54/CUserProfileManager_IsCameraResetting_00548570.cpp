struct CUserProfileManager {
    char pad0[0xc];
    char m_flagC;    // +0xc
    char pad_d;      // +0xd
    char m_flagE;    // +0xe
    char pad1[0xce - 0xf];
    char m_flagCE;   // +0xce
};

char __fastcall IsCameraResetting(CUserProfileManager* this_ptr) {
    if (this_ptr->m_flagC)
        return this_ptr->m_flagCE;
    return this_ptr->m_flagE;
}