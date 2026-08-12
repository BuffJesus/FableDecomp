#include <cstdio>

static int g_called = 0;

struct CLipSyncDataBank {
    char pad[0x178];
    unsigned char m_bSuspended; // +0x178
    void DoUpdate();
    void Update();
};

// Provide the external tail-call target locally for standalone link.
void CLipSyncDataBank::DoUpdate()
{
    g_called++;
}

void CLipSyncDataBank::Update()
{
    if (this->m_bSuspended == 0)
        this->DoUpdate();
}

int main()
{
    CLipSyncDataBank b;
    for (int i = 0; i < 0x179; i++) ((char*)&b)[i] = 0;

    // suspended == 0 -> DoUpdate runs
    b.m_bSuspended = 0;
    g_called = 0;
    b.Update();
    if (g_called != 1) { printf("FAIL a\n"); return 1; }

    // suspended != 0 -> DoUpdate skipped
    b.m_bSuspended = 5;
    g_called = 0;
    b.Update();
    if (g_called != 0) { printf("FAIL b\n"); return 1; }

    printf("LIPSYNC_UPDATE_OK\n");
    return 0;
}