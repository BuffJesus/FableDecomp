struct CActiveFile;

// __fastcall helper on self, returns nothing we use
void __fastcall CActiveFile_base(CActiveFile* self);
// cdecl free helper taking the field0 pointer
void free_helper(void* p);

struct CActiveFile {
    void* field0;
};

void __fastcall OnReadFinished(CActiveFile* self)
{
    CActiveFile_base(self);
    void* p = self->field0;
    if (p)
        free_helper(p);
}