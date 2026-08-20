// C3DAnimation2::Sizeof — sub-object's own size plus this class's fixed 0x7c.
#pragma pack(push,1)
struct Sub { int Sizeof(); };
struct T { char pad[0x30]; Sub* sub; int Sizeof(); };
#pragma pack(pop)
int T::Sizeof() { return this->sub->Sizeof() + 0x7c; }