#pragma optimize("s",on)
// Byte-exact reconstruction of CLandscapeBackgroundPatch::`vector_deleting_destructor'
// retail 0x004e41a1, __fastcall(CLandscapeBackgroundPatch*, uint flags) -> void*

struct Sub;

// scalar destructor of the subobject at offset +4 (retail 0x4e25f1), __fastcall
extern "C" void __fastcall Sub_dtor(Sub* self);

// operator delete (retail 0xbfe9bc)
void __cdecl operator delete(void* p);

struct CLandscapeBackgroundPatch { char raw[8]; };

// Model retail __fastcall(this in ecx, flags on stack) as __fastcall with an
// unused edx placeholder so that `flags` lands as the first STACK argument
// (read via test byte ptr [esp+8],1 after the push esi).
void* __fastcall vector_deleting_destructor(CLandscapeBackgroundPatch* self, int /*edx*/, unsigned int flags)
{
    Sub_dtor(reinterpret_cast<Sub*>(reinterpret_cast<char*>(self) + 4));
    if (flags & 1)
        operator delete(self);
    return self;
}