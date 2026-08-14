#pragma optimize("s",on)
// Byte-exact reconstruction of CLandscapeBackgroundPatch::`vector_deleting_destructor'
// retail 0x004da6a9, __fastcall(self /*ecx*/, uint flags /*stack*/)
// Harness rewrites __fastcall->__fastcall, so model this in ecx, a dummy in edx,
// and the real flags on the stack.

typedef unsigned int uint;

struct Sub {
    void nonvirt_dtor();
};

struct CLandscapeBackgroundPatch {
    int   field0;      // +0
    Sub   sub;         // +4
};

// scalar destructor of subobject at +4 (retail 0x00a01c10), __fastcall
extern "C" void __fastcall Sub_dtor(Sub* self);

// operator delete (retail 0x00bfe9bc), cdecl caller-cleanup
extern "C" void __cdecl op_delete(void* p);

void* __fastcall vector_deleting_destructor(CLandscapeBackgroundPatch* self, int /*edx dummy*/, uint flags)
{
    Sub_dtor(&self->sub);
    if (flags & 1)
        op_delete(self);
    return self;
}