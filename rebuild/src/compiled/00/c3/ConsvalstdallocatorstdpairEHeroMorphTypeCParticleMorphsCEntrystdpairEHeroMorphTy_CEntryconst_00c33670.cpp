// Retail 0x00c33670:
//   mov ecx, 0x143bb58
//   jmp  0xc3d3e0
// A tiny forwarder: replaces the incoming allocator pointer (ecx) with a fixed
// global object address, then tail-calls the real _Cons_val implementation,
// preserving the two stacked pair<> arguments.

struct Alloc {};
struct Pair {};

// The real implementation lives at 0xc3d3e0 (relocated call target).
extern "C" void __fastcall RealConsVal(Alloc* self, void* first, void* last);

// Fixed global allocator instance at 0x0143bb58 (relocated absolute).
extern Alloc g_allocInstance;

void __fastcall Cons_val_forwarder(Alloc* /*self*/, Pair* first, Pair* last)
{
    RealConsVal(&g_allocInstance, first, last);
}