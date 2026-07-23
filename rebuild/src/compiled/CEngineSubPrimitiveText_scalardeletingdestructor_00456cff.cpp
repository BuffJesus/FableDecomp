#pragma optimize("s",on)
struct Member { int pad[16]; };
struct CEngineSubPrimitiveText {
    char pad[0x3c];
    Member m;
};

extern "C" void __fastcall Member_dtor(Member* self);
extern "C" void __fastcall Base_dtor(CEngineSubPrimitiveText* self);
extern "C" void __cdecl op_delete(void* p);

void* __fastcall CEngineSubPrimitiveText_scalar_deleting_destructor(CEngineSubPrimitiveText* self, int, unsigned int flags)
{
    Member_dtor(&self->m);
    Base_dtor(self);
    if (flags & 1)
        op_delete(self);
    return self;
}