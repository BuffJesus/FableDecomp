#pragma optimize("s",on)
extern void __fastcall Sub_Dtor(void* p);
extern void __cdecl Op_Delete(void* p);

struct CLandscapeBackgroundPatch {
    int f0;
    int f4;
    void* f8;
};

void* __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor(CLandscapeBackgroundPatch* self, int edx, unsigned int flags) {
    Sub_Dtor((char*)self + 4);
    if (flags & 1) {
        Op_Delete(self);
    }
    return self;
}