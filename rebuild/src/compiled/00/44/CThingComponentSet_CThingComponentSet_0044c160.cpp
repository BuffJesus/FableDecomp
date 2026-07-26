extern void __cdecl eng_free(void* p);

struct CThingComponentSet {
    void* m_ptr;
    unsigned char m_flag;
};

void __fastcall CThingComponentSet_dtor(CThingComponentSet* self)
{
    eng_free(self->m_ptr);
    self->m_ptr = 0;
    self->m_flag = 0;
}