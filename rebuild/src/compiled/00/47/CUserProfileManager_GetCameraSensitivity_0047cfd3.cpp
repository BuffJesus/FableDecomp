#pragma optimize("s",on)
struct CUserProfileManager {
    char pad0[0xc];
    unsigned char useAlt;   // +0xc
    char pad1[0x2c - 0xd];
    float sens;             // +0x2c
    char pad2[0xec - 0x30];
    float sensAlt;          // +0xec
    float GetCameraSensitivity();
};

float CUserProfileManager::GetCameraSensitivity() {
    if (this->useAlt)
        return this->sensAlt;
    return this->sens;
}