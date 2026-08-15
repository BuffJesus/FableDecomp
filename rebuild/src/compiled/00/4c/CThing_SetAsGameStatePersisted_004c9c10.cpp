// Byte-exact reconstruction of CThing::SetAsGameStatePersisted @ 0x004c9c10
// Retail is __fastcall(CThing*, bool): self in ecx, bool on the stack at [esp+4].
// Sets bit 2 (mask 0x04) of the bitfield byte at +0x91, then calls 0x4c8c00 (ret 4).
//
// The harness rewrites __fastcall -> __fastcall on this source. A plain
// __fastcall(self, bool) would pass the bool in edx and let VC7.1 tail-call the
// void forwarder into a `jmp`. Retail keeps `call; ret 4` because the bool is a
// STACK arg the caller must still pop after the callee returns. To reproduce that,
// model the bool as the THIRD fastcall parameter (ecx=self, edx=dummy, stack=value)
// so it lands at [esp+4] and forces `ret 4` (no tail-call).

struct CThing;
void __fastcall CThing_notify(CThing *self);

struct CThingBits {
    unsigned char pad[0x91];
    unsigned char b0 : 1;
    unsigned char b1 : 1;
    unsigned char b2 : 1; // bit 2 = mask 0x04
    unsigned char b3 : 1;
    unsigned char b4 : 1;
    unsigned char b5 : 1;
    unsigned char b6 : 1;
    unsigned char b7 : 1;
};

// After the __fastcall->__fastcall rewrite this becomes:
//   void __fastcall SetAsGameStatePersisted(CThing* /*ecx*/, int /*edx*/, bool /*[esp+4]*/)
void __fastcall SetAsGameStatePersisted(CThing *self, int /*edx*/, bool value)
{
    ((CThingBits*)self)->b2 = value;
    CThing_notify((CThing*)self);
}