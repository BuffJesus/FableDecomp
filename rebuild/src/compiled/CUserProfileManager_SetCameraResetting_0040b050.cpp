struct CUserProfileManager {
    char pad_0[0xc];
    char m_flag;      /* +0xc */
    char pad_d[3];    /* +0xd..0xf */
    char m_field10;   /* +0x10 */
    char pad_11[0xbf];/* +0x11..0xcf */
    char m_fieldD0;   /* +0xd0 */
    void Notify();
    void Apply();
    void SetCameraResetting(char value, char doApply);
};

void CUserProfileManager::SetCameraResetting(char value, char doApply)
{
    if (this->m_flag != 0)
        this->m_fieldD0 = value;
    this->m_field10 = value;
    this->Notify();
    if (doApply != 0)
        this->Apply();
}