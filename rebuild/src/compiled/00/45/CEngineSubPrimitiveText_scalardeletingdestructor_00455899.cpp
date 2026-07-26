#pragma optimize("s",on)
struct CEngineSubPrimitiveText;
extern void __fastcall Sub_Dtor(void* p);
extern void __fastcall CEngineSubPrimitiveText_Dtor(CEngineSubPrimitiveText* self);
extern void __cdecl operator_delete(void* p);

void* __fastcall CEngineSubPrimitiveText_scalar_deleting_destructor(CEngineSubPrimitiveText* self, int edx, unsigned int flags)
{
    Sub_Dtor((char*)self + 0x74);
    CEngineSubPrimitiveText_Dtor(self);
    if (flags & 1)
        operator_delete(self);
    return self;
}