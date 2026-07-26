#pragma optimize("s",on)
// scalar deleting destructor for CEngineSubPrimitiveText
struct CEngineSubPrimitiveText { char pad[0x58]; };

extern "C" void __fastcall Sub54_dtor(void* ecx);
extern "C" void __fastcall CEngineSubPrimitiveText_realdtor(void* ecx);
void operator delete(void* p);

void* __fastcall CEngineSubPrimitiveText_scalar_deleting_destructor(CEngineSubPrimitiveText* self, int edx, unsigned int flags)
{
    (void)edx;
    Sub54_dtor((char*)self + 0x54);
    CEngineSubPrimitiveText_realdtor(self);
    if (flags & 1)
        operator delete(self);
    return self;
}