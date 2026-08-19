// `return !this->flag;` on a bool member — VC7.1 emits mov al / test al,al / sete al.
#pragma pack(push,1)
struct T { char pad[0xbc]; bool flag; bool CanRelocateData(); };
#pragma pack(pop)
bool T::CanRelocateData() { return !this->flag; }
