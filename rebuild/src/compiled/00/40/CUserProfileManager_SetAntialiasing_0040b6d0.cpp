struct CUserProfileManager {
    char pad0[0x0c];
    char flag0c;
    char pad1[0x4c - 0x0d];
    int val4c;
    char pad2[0x108 - 0x50];
    int val108;
};

extern void __fastcall CUserProfileManager_Apply(CUserProfileManager* self);
extern void __fastcall CUserProfileManager_Notify(CUserProfileManager* self);

void __fastcall CUserProfileManager_SetAntialiasing(CUserProfileManager* self, void* edx, int value, char doNotify)
{
    if (self->flag0c)
        self->val108 = value;
    self->val4c = value;
    CUserProfileManager_Apply(self);
    if (doNotify)
        CUserProfileManager_Notify(self);
}