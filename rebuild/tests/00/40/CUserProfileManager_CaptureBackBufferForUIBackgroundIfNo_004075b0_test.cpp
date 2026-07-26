#include <cstdio>

struct CUserProfileManager { char pad[0x138]; void* m_pBackBuffer; };

static int g_called = 0;
void __fastcall CUserProfileManager_CaptureBackBufferForUIBackground(CUserProfileManager* self) { (void)self; g_called = 1; }

void __fastcall CUserProfileManager_CaptureBackBufferForUIBackgroundIfNonExistant(CUserProfileManager* self)
{
    if (self->m_pBackBuffer == 0)
        CUserProfileManager_CaptureBackBufferForUIBackground(self);
}

int main()
{
    CUserProfileManager a;
    a.m_pBackBuffer = 0;
    g_called = 0;
    CUserProfileManager_CaptureBackBufferForUIBackgroundIfNonExistant(&a);
    if (g_called != 1) { std::printf("FAIL: expected call when null\n"); return 1; }

    int dummy = 5;
    a.m_pBackBuffer = &dummy;
    g_called = 0;
    CUserProfileManager_CaptureBackBufferForUIBackgroundIfNonExistant(&a);
    if (g_called != 0) { std::printf("FAIL: unexpected call when non-null\n"); return 1; }

    std::printf("CUserProfileManager_004075b0_TEST PASS\n");
    return 0;
}