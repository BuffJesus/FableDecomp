// CActiveFile::OnReadFinished @ 007cbc20
struct CActiveFile {
    void* m_ptr;
};

// __fastcall base call at 0x7cbb80
void __fastcall OnReadFinishedBase(CActiveFile* self);
// cdecl free function at 0xbfea14
void FreeBuffer(void* p);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    OnReadFinishedBase(self);
    void* p = self->m_ptr;
    if (p)
        FreeBuffer(p);
}