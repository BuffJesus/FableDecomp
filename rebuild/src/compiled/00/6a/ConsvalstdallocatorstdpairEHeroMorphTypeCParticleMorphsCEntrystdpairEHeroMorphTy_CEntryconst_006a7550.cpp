// Byte-exact reconstruction of 0x006a7550
//   mov edx,[ecx]      ; edx = *self
//   mov ecx,[esp+4]    ; ecx = pair* stack arg
//   call 0x76d2f0      ; g(ecx=arg, edx=*self)
//   ret 4
// Outer: this in ecx, ONE stack arg -> model as __fastcall(self, edx_dummy, arg)
// so self->ecx, dummy->edx (dead, overwritten), arg->[esp+4]; ret 4.

struct Pair;
struct Payload;

struct Allocator {
    Payload* m_first;   // +0x00
};

// callee 0x76d2f0: __fastcall(arg /*ecx*/, payload /*edx*/)
extern "C" Payload* __fastcall callee_76d2f0(Pair* arg, Payload* payload);

Payload* __fastcall Cons_val(Allocator* self, int /*edx*/, Pair* arg)
{
    return callee_76d2f0(arg, self->m_first);
}