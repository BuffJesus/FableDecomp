// Reconstruction of retail 0x00c20560
// __fastcall-style: this in ECX, one long arg on stack, ret 4.
// call helper(&this->second, a) [helper is this-in-ecx, arg on stack]; this->flags |= 0x40;

struct Inner {
    void Store(long a); // __fastcall -> this in ECX, a on stack (target 0x00c1bc00)
};

struct Obj {
    unsigned long flags;   // +0x00
    long          pad;     // +0x04
    Inner         second;  // +0x08
};

// this in ECX; second arg on stack (dummy EDX to force stack placement) -> ret 4
void __fastcall Recon(Obj* self, int /*edx*/, long a)
{
    self->second.Store(a);
    self->flags |= 0x40;
}