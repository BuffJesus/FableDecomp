// Clear a byte flag, then tail-call a member of a fixed global object (0x013CAA90).
// __fastcall this=ecx, no args.
struct Iface { void Add(); };
extern Iface* g_iface;
#pragma pack(push,1)
struct T { char pad[0x44]; char flag; void AddToInterface(); };
#pragma pack(pop)
void T::AddToInterface() {
    this->flag = 0;
    g_iface->Add();
}
