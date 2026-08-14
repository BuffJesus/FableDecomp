#pragma optimize("s",on)
// Byte-exact reconstruction of CAnimateThing::SetAsFinished @ 0x00c0a860
// Retail thiscall reads its single argument from the stack ([esp+4]) rather
// than edx, so model it as __fastcall with a dummy edx slot forcing the real
// argument onto the stack.
struct CAnimateThing {
    char pad[0x48];
    // 0x26: flag byte, 0x48: stored value
};

void __fastcall SetAsFinished(CAnimateThing *self, int /*edx*/, void *value)
{
    *((unsigned char *)self + 0x26) |= 0x20;
    *(void **)((char *)self + 0x48) = value;
}