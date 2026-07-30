
#include <cstdio>
#include <cstring>

struct CTCKraken {
    char pad[0x1a8];
    int  m_phase;      // +0x1a8
    int  m_remaining;  // +0x1ac
};

char __fastcall HaveAllTentaclesBeenHit(CTCKraken *self)
{
    if (self->m_phase != 2)
        return 0;
    return self->m_remaining == 0;
}

int main()
{
    CTCKraken k;
    memset(&k, 0, sizeof(k));

    // phase != 2 -> 0, regardless of remaining
    k.m_phase = 0; k.m_remaining = 0;
    if (HaveAllTentaclesBeenHit(&k) != 0) { printf("FAIL case1\n"); return 1; }

    k.m_phase = 1; k.m_remaining = 5;
    if (HaveAllTentaclesBeenHit(&k) != 0) { printf("FAIL case2\n"); return 1; }

    // phase == 2, remaining != 0 -> 0
    k.m_phase = 2; k.m_remaining = 3;
    if (HaveAllTentaclesBeenHit(&k) != 0) { printf("FAIL case3\n"); return 1; }

    // phase == 2, remaining == 0 -> 1
    k.m_phase = 2; k.m_remaining = 0;
    if (HaveAllTentaclesBeenHit(&k) != 1) { printf("FAIL case4\n"); return 1; }

    printf("OK_0x00557940\n");
    return 0;
}