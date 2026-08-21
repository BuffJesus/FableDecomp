#pragma optimize("s",on)
// Constant-return member: returns 56. __fastcall this=ecx.
struct T { int Get(); };
int T::Get() { return 56; }