#pragma optimize("s",on)
// Byte-exact reconstruction of CLandscapeBackgroundPatch::`vector_deleting_destructor'
// retail 0x00656e8e, __fastcall(void*)(CLandscapeBackgroundPatch* this, unsigned int flags)
//
// Shape: scalar/vector deleting destructor.
//   push esi; mov esi,ecx
//   lea ecx,[esi+0x10]; call <member-dtor at +0x10>
//   test [esp+8],1 ; je +; push esi; call operator delete; pop ecx
//   mov eax,esi; pop esi; ret 4

struct SubObj { char pad[4]; };

// member subobject destructor at this+0x10 (thiscall -> fastcall self in ecx)
extern void __fastcall SubObj_dtor(SubObj* self);   // 0x440c40

// operator delete (cdecl)
extern "C" void __cdecl eng_operator_delete(void* p); // 0xbfe9bc

struct CLandscapeBackgroundPatch
{
    char   pad0[0x10];   // +0x00
    SubObj sub;          // +0x10
};

// __fastcall: this in ecx, flags on stack. Model as fastcall with an edx dummy
// so that 'flags' lands on the stack at [esp+8] like retail.
void* __fastcall LBP_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edxDummy, unsigned int flags)
{
    (void)edxDummy;
    SubObj_dtor(&self->sub);
    if (flags & 1)
        eng_operator_delete(self);
    return self;
}