// Byte-exact reconstruction of _Cons_val<std::allocator<C3DGeneratorObject>,...>
// retail 0x00c918a0

struct C3DGeneratorObject {
    unsigned long m[0x84];
};

struct alloc_C3DGeneratorObject {};

// Placement ctor invoked via an indirect call through a global fn-ptr
// (retail: call dword ptr [0x1440154]) with cdecl cleanup (push ptr; call; add esp,4).
typedef C3DGeneratorObject* (__cdecl *ctor_fp)(C3DGeneratorObject* p);
extern ctor_fp g_C3DGeneratorObject_ctor;

// __fastcall: self(alloc) in ecx, _Ptr in edx. Two params => ret (no stack cleanup).
void __fastcall Cons_val(alloc_C3DGeneratorObject* self, C3DGeneratorObject* _Ptr)
{
    C3DGeneratorObject* p = (C3DGeneratorObject*)self;
    if (p) {
        for (int i = 0; i < 0x84; ++i) ((unsigned long*)p)[i] = 0;
        g_C3DGeneratorObject_ctor(p);
    }
}