// CActiveFile::OnReadFinished @ 0070ba70
struct CActiveFile;

extern "C" void __fastcall CActiveFile_base_0x70ba00(CActiveFile* self);
extern "C" void CActiveFile_free_0xbfea14(void* p);

struct CActiveFile {
    void* m_ptr; // offset 0
};

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_base_0x70ba00(self);
    void* p = self->m_ptr;
    if (p != 0)
        CActiveFile_free_0xbfea14(p);
}