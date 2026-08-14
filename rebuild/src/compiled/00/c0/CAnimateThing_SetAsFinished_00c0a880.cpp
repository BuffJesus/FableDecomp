#pragma optimize("s",on)
struct CAnimateThing {
    char pad[0x100];
};

// retail is __fastcall: ecx=this, arg on stack [esp+4], ret 4.
// Model with __fastcall + dummy edx so the real bool lands on the stack.
void __fastcall SetAsFinished(CAnimateThing *self, int edx_dummy, bool finished)
{
    unsigned char *p = (unsigned char *)self;
    p[0x26] |= 0x80;
    p[0x54] = (unsigned char)finished;
}