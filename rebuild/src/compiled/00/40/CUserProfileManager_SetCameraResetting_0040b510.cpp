struct CUserProfileManager {
    char pad0[0xc];
    char gate;        /* +0xc */
    char pad1[0x79 - 0xd];
    char flag79;      /* +0x79 */
    char pad2[0x12c - 0x7a];
    char flag12c;     /* +0x12c */
};

extern void __fastcall CUserProfileManager_HelperA(CUserProfileManager* self);
extern void __fastcall CUserProfileManager_HelperB(CUserProfileManager* self);

void __fastcall CUserProfileManager_SetCameraResetting(CUserProfileManager* self, void* /*edx*/, char p1, char p2)
{
    if (self->gate)
        self->flag12c = p1;
    self->flag79 = p1;
    CUserProfileManager_HelperA(self);
    if (p2)
        CUserProfileManager_HelperB(self);
}