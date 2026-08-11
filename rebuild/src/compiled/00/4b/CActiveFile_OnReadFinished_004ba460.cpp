// CActiveFile::OnReadFinished @ 0x004ba460
struct CActiveFile;

extern "C" void __cdecl fable_free(void* p);
void __fastcall CActiveFileBase_OnReadFinished(CActiveFile* self);

struct CActiveFile {
    void* field0;
};

void __fastcall OnReadFinished(CActiveFile* self)
{
    CActiveFileBase_OnReadFinished(self);
    void* p = self->field0;
    if (p != 0)
        fable_free(p);
}