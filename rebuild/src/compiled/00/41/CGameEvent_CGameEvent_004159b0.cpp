
struct CGameEventT {
    void* p;           /* +0 */
    unsigned char b4;  /* +4 */
    char pad[0x25-5];
    unsigned char b25; /* +0x25 */
    unsigned char b26; /* +0x26 */
    unsigned char b27; /* +0x27 */
};

/* __fastcall ctor: this in ecx, single arg on the STACK, returns this in eax, ret 4.
   Emulated via __fastcall(this=ecx, edx_dummy, arg=stack) returning this. */
CGameEventT* __fastcall CGameEvent(CGameEventT* self, int /*edx*/, void* arg)
{
    self->p = arg;
    self->b4 = 0xff;
    self->b25 = 0;
    self->b26 = 1;
    self->b27 = 0;
    return self;
}