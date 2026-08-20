// One-byte local zeroed through the `rep stosb`-style aggregate-init path and read back.
// VC7.1 emits `xor eax,eax; lea edi,[ebp-1]; stosb; mov al,[ebp-1]` for a 1-element
// char array cleared with a counted store. __fastcall this=ecx, no args.
struct T { bool AddChildPrimitive(); };
bool T::AddChildPrimitive() {
    char buf[1];
    for (int i = 0; i < 1; ++i) buf[i] = 0;
    return buf[0] != 0;
}
