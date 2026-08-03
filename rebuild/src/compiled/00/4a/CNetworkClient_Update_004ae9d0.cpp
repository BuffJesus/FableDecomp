// CNetworkClient::Update @ 0x004ae9d0  (__fastcall, ret 0xc)
struct CNetworkClient {
    char _pad0[0x2662];
    unsigned char enabled;   // +0x2662
    char _pad1[0x266c - 0x2663];
    int  a;                  // +0x266c
    int  b;                  // +0x2670
    int  c;                  // +0x2674
};

void __fastcall Update(CNetworkClient *self, void * /*edx*/, int a, int b, int c)
{
    if (self->enabled) {
        self->a = a;
        self->b = b;
        self->c = c;
    }
}