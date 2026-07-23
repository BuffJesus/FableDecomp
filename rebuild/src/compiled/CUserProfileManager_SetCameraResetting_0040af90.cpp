struct CUserProfileManager {
    char pad_c[0xc];
    char flag_c;
    char field_d;
    char pad_e[0xcd - 0xe];
    char field_cd;
    void ApplyCamera();
    void NotifyReset();
    void SetCameraResetting(char a1, char a2);
};

void CUserProfileManager_SetCameraResetting(CUserProfileManager* self, char a1, char a2);

void CUserProfileManager::SetCameraResetting(char a1, char a2)
{
    if (this->flag_c)
        this->field_cd = a1;
    this->field_d = a1;
    this->ApplyCamera();
    if (a2)
        this->NotifyReset();
}