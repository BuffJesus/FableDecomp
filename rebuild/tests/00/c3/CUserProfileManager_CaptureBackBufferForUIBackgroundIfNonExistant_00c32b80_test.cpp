#include <cstdio>

struct CUserProfileManager
{
    int   m_0;
    int   m_4;
    unsigned int m_8;

    void Sibling_00c32950(unsigned int pos);
    void CaptureBackBufferForUIBackgroundIfNonExistant(unsigned int pos);
};

static unsigned int g_last = 0xFFFFFFFF;
static int g_called = 0;

void CUserProfileManager::Sibling_00c32950(unsigned int pos)
{
    g_called = 1;
    g_last = pos;
}

int main()
{
    CUserProfileManager o;
    o.m_0 = 0; o.m_4 = 0; o.m_8 = 100;

    g_called = 0; g_last = 0xFFFFFFFF;
    o.CaptureBackBufferForUIBackgroundIfNonExistant(100);
    if (g_called != 0) { printf("FAIL early\n"); return 1; }

    o.CaptureBackBufferForUIBackgroundIfNonExistant(50);
    if (g_called != 0) { printf("FAIL early2\n"); return 1; }

    g_called = 0; g_last = 0xFFFFFFFF;
    o.CaptureBackBufferForUIBackgroundIfNonExistant(150);
    if (g_called != 1 || g_last != 50) { printf("FAIL sub\n"); return 1; }

    printf("PROFILE_CAPTURE_OK\n");
    return 0;
}