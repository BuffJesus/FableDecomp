// Constant-return member: returns false. __fastcall this=ecx.
struct T { bool Get(); };
bool T::Get() { return false; }