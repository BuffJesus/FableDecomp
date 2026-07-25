struct CNetworkClientUpdateOverlay
{
    std::byte Padding0[0x2662];
    bool LocalGame;
    std::byte Padding2663[0x266C - 0x2663];
    long LocalFrame;
    unsigned long Checksum1;
    unsigned long Checksum2;
};

static_assert(offsetof(CNetworkClientUpdateOverlay, LocalGame) == 0x2662);
static_assert(offsetof(CNetworkClientUpdateOverlay, LocalFrame) == 0x266C);
static_assert(offsetof(CNetworkClientUpdateOverlay, Checksum1) == 0x2670);
static_assert(offsetof(CNetworkClientUpdateOverlay, Checksum2) == 0x2674);

void CNetworkClient::Update(long time_stamp, unsigned long checksum1, unsigned long checksum2)
{
    auto& self = *reinterpret_cast<CNetworkClientUpdateOverlay*>(this);

    if (self.LocalGame != false)
    {
        self.LocalFrame = time_stamp;
        self.Checksum1 = checksum1;
        self.Checksum2 = checksum2;
    }
}