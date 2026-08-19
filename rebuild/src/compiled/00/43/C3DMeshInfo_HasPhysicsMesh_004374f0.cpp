// `return this->count != 1;` — cmp/setne on a member at this+0x294. __fastcall this=ecx.
#pragma pack(push,1)
struct T { char pad[0x294]; int count; bool HasPhysicsMesh(); };
#pragma pack(pop)
bool T::HasPhysicsMesh() { return this->count != 1; }