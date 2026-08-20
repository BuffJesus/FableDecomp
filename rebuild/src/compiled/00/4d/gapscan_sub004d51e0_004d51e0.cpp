// Constant-return member: returns 0xbb. __fastcall this=ecx.
struct T { int Get(); };
int T::Get() { return 0xbb; }