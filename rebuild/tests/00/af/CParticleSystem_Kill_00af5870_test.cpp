#include <stdio.h>

struct CParticleSystem {
    char pad[0x50];
    unsigned char flags;   // +0x50

    void Stop();
    void Reset();
    void Kill();
};

static int g_stop = 0;
static int g_reset = 0;
void CParticleSystem::Stop()  { g_stop++; }
void CParticleSystem::Reset() { g_reset++; }

void CParticleSystem::Kill()
{
    this->flags &= (unsigned char)0xFE;
    this->Stop();
    this->Reset();
    this->flags |= (unsigned char)0x04;
}

int main()
{
    CParticleSystem ps;
    ps.flags = 0xFF;          // all bits set
    ps.Kill();
    // bit0 cleared, bit2 set: 0xFF & 0xFE = 0xFE, | 0x04 = 0xFE
    int ok1 = (ps.flags == 0xFE) && (g_stop == 1) && (g_reset == 1);

    ps.flags = 0x00;          // all bits clear
    g_stop = 0; g_reset = 0;
    ps.Kill();
    // 0x00 & 0xFE = 0x00, | 0x04 = 0x04
    int ok2 = (ps.flags == 0x04) && (g_stop == 1) && (g_reset == 1);

    ps.flags = 0x01;          // only bit0 set
    ps.Kill();
    // 0x01 & 0xFE = 0x00, | 0x04 = 0x04
    int ok3 = (ps.flags == 0x04);

    if (ok1 && ok2 && ok3) { printf("KILL_OK\n"); return 0; }
    printf("KILL_FAIL f=%02x s=%d r=%d\n", ps.flags, g_stop, g_reset);
    return 1;
}