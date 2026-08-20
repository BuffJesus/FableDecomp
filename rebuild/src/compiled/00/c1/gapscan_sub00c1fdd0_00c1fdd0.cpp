// Constant-return member: returns 0x3. __fastcall this=ecx.
struct T { int Get(); };
int T::Get() { return 0x3; }