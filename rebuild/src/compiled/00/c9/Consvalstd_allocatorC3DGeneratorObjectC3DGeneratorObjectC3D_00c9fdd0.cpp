// Byte-exact reconstruction of 0x00c9fdd0
// _Cons_val<std::allocator<C3DGeneratorObject>,C3DGeneratorObject,C3DGeneratorObject>
//
// Retail disasm:
//   mov edx,ecx; test edx,edx; je ret; push edi; xor eax,eax; mov edi,edx;
//   mov ecx,0x1c5; push edx; rep stosd; call [0x1440154]; add esp,4; pop edi; ret
//
// Semantics: if the destination object pointer (ecx) is non-null, zero 0x1c5
// (453) dwords = 1812 bytes at it, then call a cdecl global function pointer
// with the object pointer as its single (caller-cleaned) argument. The two
// C3DGeneratorObject* params are unreferenced by codegen; model as a __fastcall
// free fn whose only used arg is the object in ecx.

typedef unsigned long ulong;

// cdecl global function pointer ([0x1440154]) — object initializer.
void (__cdecl *g_initObject)(void *obj);

struct C3DGeneratorObject {
    ulong words[0x1c5]; // 453 dwords = 1812 bytes
};

void __fastcall Cons_val_C3DGeneratorObject(C3DGeneratorObject *self)
{
    if (self == 0)
        return;
    ulong *p = self->words;
    for (int i = 0; i < 0x1c5; ++i)
        p[i] = 0;
    g_initObject((void *)self);
}