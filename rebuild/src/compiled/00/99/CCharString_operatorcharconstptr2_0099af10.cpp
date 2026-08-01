struct CCharString
{
    void* data;
};

extern "C" char g_empty;

char* __fastcall CCharString_operator_char_const_ptr(CCharString* self, void* /*edx_unused*/)
{
    void* data = self->data;
    if (!data)
    {
        return &g_empty;
    }
    return *(char**)data;
}