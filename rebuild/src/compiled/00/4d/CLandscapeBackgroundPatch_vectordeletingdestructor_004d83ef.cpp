#pragma optimize("s", on)
struct AutoTinyVectorObject
{
    void* vftable;
};
extern void* const g_AutoTinyVectorVftable;
extern void __fastcall AutoTinyVectorDestructor(void* self);
extern void __cdecl AutoTinyVectorDelete(void* object);
void* __fastcall vector_deleting_destructor(AutoTinyVectorObject* self, int, unsigned int flags)
{
    self->vftable = (void*)&g_AutoTinyVectorVftable;
    AutoTinyVectorDestructor(self);
    if (flags & 1)
        AutoTinyVectorDelete(self);
    return self;
}