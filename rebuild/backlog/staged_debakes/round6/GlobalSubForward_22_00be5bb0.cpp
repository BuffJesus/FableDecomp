// Forward (this, arg) to a member of the sub-object at global(0x01436EA8)+0x6b0.
// __fastcall this=ecx, arg=edx; both are re-pushed as ordinary stack args, so retail
// parks `this` in eax first (`mov eax,ecx`) and pushes edx then eax.
struct Target { void Run(void* self, void* arg); };
#pragma pack(push,1)
struct Root { char pad[0x6b0]; Target* target; };
#pragma pack(pop)
extern Root* g_root;
extern "C" void __fastcall Forward(void* self, void* arg) {
    g_root->target->Run(self, arg);
}
