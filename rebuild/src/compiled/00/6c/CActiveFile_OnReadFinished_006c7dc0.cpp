// CActiveFile::OnReadFinished @ 006c7dc0
struct CActiveFile {
    void* m_thing;   // +0x00
};

void __fastcall CActiveFile_Base(CActiveFile* self);   // 0x6c7670
void CActiveFile_FreeThing(void* p);                   // 0xbfea14 (cdecl)

void __fastcall CActiveFile__OnReadFinished(CActiveFile* self)
{
    CActiveFile_Base(self);
    void* p = self->m_thing;
    if (p != 0)
        CActiveFile_FreeThing(p);
}