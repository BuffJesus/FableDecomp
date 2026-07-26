struct CUserProfileManager {
    char pad0[0xc];
    unsigned char flagC;   // +0xc
    char pad1[0x9];
    unsigned char byte16;  // +0x16
    char pad2[0xbf];
    unsigned char byteD6;  // +0xd6
};

extern void __fastcall UPM_helperA(CUserProfileManager* self);
extern void __fastcall UPM_helperB(CUserProfileManager* self);

void __fastcall CUserProfileManager_SetCameraResetting(CUserProfileManager* self, void* /*edx*/, char p1, char p2)
{
    if (self->flagC)
        self->byteD6 = (unsigned char)p1;
    self->byte16 = (unsigned char)p1;
    UPM_helperA(self);
    if (p2)
        UPM_helperB(self);
}