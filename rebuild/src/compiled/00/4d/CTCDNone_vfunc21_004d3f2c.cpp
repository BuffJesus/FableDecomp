#pragma optimize("s",on)
// Constant-return member: returns 96. __fastcall this=ecx.
struct T { int Get(); };
int T::Get() { return 96; }