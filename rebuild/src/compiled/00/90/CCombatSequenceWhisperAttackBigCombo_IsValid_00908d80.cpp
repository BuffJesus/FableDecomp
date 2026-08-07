struct Inner { char pad[0x6c]; unsigned char b6c; };
struct S { char pad[0x20]; Inner* f20; };
unsigned char __fastcall IsValid(S* self){ unsigned char b = self->f20->b6c; b >>= 5; b = ~b; b &= 1; return b; }