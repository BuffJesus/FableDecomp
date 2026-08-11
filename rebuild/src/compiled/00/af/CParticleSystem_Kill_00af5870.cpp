// CParticleSystem::Kill  @ 0x00af5870  (__fastcall, void)

struct CParticleSystem {
    char pad[0x50];
    unsigned char flags;   // +0x50

    void Stop();     // 0xaf56e0  (defined elsewhere -> real call emitted)
    void Reset();    // 0xaf5770  (defined elsewhere -> real call emitted)
    void Kill();
};

void CParticleSystem::Kill()
{
    this->flags &= (unsigned char)0xFE;
    this->Stop();
    this->Reset();
    this->flags |= (unsigned char)0x04;
}