extern "C" void __cdecl eng_free(void* p);

struct CThingComponentSet {
    void* buf;
    char flag;
};

void __fastcall CThingComponentSet_dtor(CThingComponentSet* self)
{
    eng_free(self->buf);
    self->buf = 0;
    self->flag = 0;
}