struct CUserProfileManager {
    char pad0[0xc];
    char flag_c;                 /* +0x0c */
    char pad1[0x19 - 0x0c - 1];
    char at19;                   /* +0x19 */
    char pad2[0xd9 - 0x19 - 1];
    char atD9;                   /* +0xd9 */
    void Helper1();
    void Helper2();
    void SetCameraResetting(char a, char b);
};

void CUserProfileManager_SetCameraResetting(CUserProfileManager* self, char a, char b);

void CUserProfileManager::SetCameraResetting(char a, char b)
{
    if (this->flag_c)
        this->atD9 = a;
    this->at19 = a;
    this->Helper1();
    if (b)
        this->Helper2();
}