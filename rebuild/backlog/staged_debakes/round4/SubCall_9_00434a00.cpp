// Call a member of the sub-OBJECT at this+0x70 (address adjust, not a pointer load).
// __fastcall this=ecx, no args.
#pragma pack(push,1)
struct Sub { int Run(); };
struct T { char pad[0x70]; Sub sub; void Kick(); };
#pragma pack(pop)
void T::Kick() { this->sub.Run(); }
