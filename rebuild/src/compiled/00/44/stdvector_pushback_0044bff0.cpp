struct CWideString
{
    unsigned long value;
    void CopyConstruct(CWideString* source);
};

struct VectorPushBackOverlay
{
    unsigned char unused[4];
    CWideString* finish;
    CWideString* end_of_storage;

    void InsertRange(
        CWideString* position,
        CWideString* value,
        CWideString** value_copy,
        int count,
        int copies);
};

void __fastcall push_back(VectorPushBackOverlay* self, void*, CWideString* value)
{
    CWideString* finish = self->finish;
    if (finish != self->end_of_storage) {
        if (finish != 0) {
            finish->CopyConstruct(value);
        }
        self->finish = (CWideString*)((unsigned char*)self->finish + 4);
        return;
    }
    self->InsertRange(finish, value, &value, 1, 1);
}