// Byte-exact reconstruction of CLandscapeBackgroundPatch::`vector deleting destructor'
// retail 0x0047b300, __fastcall(self, uint flags) -> void*
//
// Pattern:
//   push esi; mov esi,ecx
//   lea ecx,[esi+4]; call innerDtor   ; scalar dtor of member/base at +4
//   test byte[esp+8],1; je S
//   push esi; call operator delete; add esp,4
// S: mov eax,esi; pop esi; ret 4

struct Sub;                 // subobject at offset +4
struct CLandscapeBackgroundPatch {
    int   m_vtbl;           // +0  (vtable / first dword)
    Sub*  dummy;            // placeholder so member sits at +4
};

// scalar destructor operating on the subobject located at self+4.
// __fastcall => arg1 in ecx. Single pointer param = ecx.
extern "C" void __fastcall InnerDtor(void* subAtPlus4);

// operator delete (cdecl)
extern "C" void __cdecl EngineOperatorDelete(void* p);

// __fastcall: this in ecx, flags on the stack. Model via __fastcall with an
// unused edx dummy so the real `flags` param lands on the stack (=> ret 4).
extern "C" void* __fastcall VecDelDtor(CLandscapeBackgroundPatch* self, int edxDummy, unsigned int flags)
{
    (void)edxDummy;
    // scalar dtor on the subobject at self+4
    InnerDtor((char*)self + 4);
    if (flags & 1)
        EngineOperatorDelete(self);
    return self;
}