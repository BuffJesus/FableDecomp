#pragma optimize("s",on)
extern "C" void __cdecl free_block(void* p);

struct CSystemRegistry {
    void* field0;
    void* field4;
    void* field8;
    void* fieldC;
    void* field10;
};

void __fastcall CSystemRegistry_dtor(CSystemRegistry* self)
{
    if (self->field10)
        free_block(self->field10);
    if (self->field0)
        free_block(self->field0);
}