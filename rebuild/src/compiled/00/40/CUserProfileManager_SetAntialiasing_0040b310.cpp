struct CUserProfileManager {
    char pad0[0x0c];
    char flag0c;
    char pad1[0x13];
    int  val20;
    char pad2[0xbc];
    int  vale0;
};

extern void __fastcall CUserProfileManager_Apply(CUserProfileManager* self);
extern void __fastcall CUserProfileManager_Refresh(CUserProfileManager* self);

void __fastcall CUserProfileManager_SetAntialiasing(CUserProfileManager* self, int edx, volatile int val, char flag)
{
    if (self->flag0c)
        self->vale0 = val;
    self->val20 = val;
    CUserProfileManager_Apply(self);
    if (flag)
        CUserProfileManager_Refresh(self);
}