#include <cstdio>

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

int main() {
    CUserProfileManager m;
    m.sens = 1.5f;
    m.sensAlt = 9.25f;

    m.useAlt = 0;
    float a = m.GetCameraSensitivity();

    m.useAlt = 1;
    float b = m.GetCameraSensitivity();

    m.useAlt = 5;
    float c = m.GetCameraSensitivity();

    if (a == 1.5f && b == 9.25f && c == 9.25f) {
        printf("SENS_OK\n");
        return 0;
    }
    printf("SENS_FAIL a=%f b=%f c=%f\n", a, b, c);
    return 1;
}