extern "C" void __cdecl fable_op_delete(void* p);

struct CThingComponentSet {
    void* m_ptr;
    char  m_flag;
};

void __fastcall CThingComponentSet_dtor(CThingComponentSet* self)
{
    fable_op_delete(self->m_ptr);
    self->m_ptr = 0;
    self->m_flag = 0;
}