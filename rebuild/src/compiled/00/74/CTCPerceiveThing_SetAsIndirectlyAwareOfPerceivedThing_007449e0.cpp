// Byte-exact reconstruction of CTCPerceiveThing::SetAsIndirectlyAwareOfPerceivedThing @ 0x007449e0

struct CTCPerceiveThing;

// callee at 0xa01b50: takes (this+0xc) in ecx, returns int
extern int __fastcall Sub_a01b50(void* self);
// callee at 0x744730: void member, this in ecx
extern void __fastcall Do_744730(CTCPerceiveThing* self);

struct CTCPerceiveThing {
    char pad0[0x2d];
    unsigned char flag2d;     // +0x2d
};

void __fastcall CTCPerceiveThing_SetAsIndirectlyAwareOfPerceivedThing(CTCPerceiveThing* self)
{
    if (Sub_a01b50((char*)self + 0xc) == 0)
        return;
    if (self->flag2d != 0)
        return;
    Do_744730(self);
}