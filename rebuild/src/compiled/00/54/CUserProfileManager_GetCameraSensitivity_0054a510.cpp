// CUserProfileManager::GetCameraSensitivity @ 0x0054a510
// __fastcall, returns float in st(0)
struct CUserProfileManager {
    char pad0[0xc];
    char useAlternate;   // +0xc
    char pad1[0x7c - 0xd];
    float sensA;         // +0x7c
    char pad2[0x130 - 0x80];
    float sensB;         // +0x130
};

float __fastcall GetCameraSensitivity(CUserProfileManager* self)
{
    if (self->useAlternate)
        return self->sensB;
    return self->sensA;
}