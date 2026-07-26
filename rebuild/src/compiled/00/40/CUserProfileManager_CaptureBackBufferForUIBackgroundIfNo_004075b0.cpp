struct CUserProfileManager { char pad[0x138]; void* m_pBackBuffer; };
extern void __fastcall CUserProfileManager_CaptureBackBufferForUIBackground(CUserProfileManager* self);

void __fastcall CUserProfileManager_CaptureBackBufferForUIBackgroundIfNonExistant(CUserProfileManager* self)
{
    if (self->m_pBackBuffer == 0)
        CUserProfileManager_CaptureBackBufferForUIBackground(self);
}