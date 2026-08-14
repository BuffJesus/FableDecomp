#pragma optimize("s",on)
// Byte-exact reconstruction of CLandscapeBackgroundPatch::`vector deleting destructor'
// retail 0x005799fd, __fastcall(self, uint flags) -> void*
//
// disasm:
//   push esi; mov esi,ecx
//   lea ecx,[esi+4]; call 0x5797fa   (destructor of sub-object at +4)
//   test byte ptr [esp+8],1; je end
//   push esi; call operator delete; pop ecx
//   end: mov eax,esi; pop esi; ret 4

struct CLandscapeBackgroundPatch;

// Sub-object destructor at esi+4 (retail 0x5797fa). Modeled as an external
// __fastcall taking the sub-object pointer in ecx.
extern "C" void __fastcall sub_dtor_5797fa(void* self);

// operator delete (retail 0xbfe9bc)
extern "C" void __cdecl eng_operator_delete(void* p);

// Model __fastcall: this in ecx, flags on the STACK. Under the harness's
// __fastcall->__fastcall rewrite, a leading dummy edx param forces the real
// flags arg onto the stack, giving the retail `test [esp+8],1` + `ret 4`.
extern "C" void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx_dummy, unsigned int flags)
{
    // call sub-object destructor on this+4
    sub_dtor_5797fa((char*)self + 4);
    if (flags & 1)
        eng_operator_delete(self);
    return self;
}