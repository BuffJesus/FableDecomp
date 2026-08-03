#pragma optimize("s",on)
struct CUserProfileManager {
    char pad0[0xc];
    char m_flag;        // +0x0c
    char m_reset;       // +0x0d
    char pad1[0xcd - 0x0e];
    char m_resetAlt;    // +0xcd
};

char __fastcall IsCameraResetting(CUserProfileManager *self)
{
    if (self->m_flag)
        return self->m_resetAlt;
    return self->m_reset;
}