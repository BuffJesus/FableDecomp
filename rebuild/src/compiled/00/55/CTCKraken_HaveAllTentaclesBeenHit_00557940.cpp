
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