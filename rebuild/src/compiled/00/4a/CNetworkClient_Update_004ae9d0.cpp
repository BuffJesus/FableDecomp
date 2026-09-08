// CNetworkClient::Update @ 0x004ae9d0  (__fastcall, ret 0xc)

#include "engine/CNetworkClient.h"  // retyped onto the PDB layout; byte parity re-verified
void __fastcall Update(CNetworkClient *self, void * /*edx*/, int a, int b, int c)
{
    if (self->LocalGame) {
        self->LocalFrame = a;
        self->Checksum1 = b;
        self->Checksum2 = c;
    }
}