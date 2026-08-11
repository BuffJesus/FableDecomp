// CActiveFile::OnReadFinished @ 0x0075f7a0
extern "C" void __cdecl free(void*);

struct CActiveFile {
    void* buffer; // +0x00
};

// __fastcall helper at 0x0075f580
void __fastcall CActiveFile_Base(CActiveFile* self);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_Base(self);
    void* p = self->buffer;
    if (p)
        free(p);
}