// CActiveFile::OnReadFinished @ 006c2d20
struct CActiveFile {
    void* field0;
};

// base method at 0x6c2c00 (__fastcall on this)
void __fastcall CActiveFile_base(CActiveFile* self);
// free/cdecl function at 0xbfea14 taking one pointer arg
void CActiveFile_free(void* p);

void __fastcall CActiveFile__OnReadFinished(CActiveFile* self)
{
    CActiveFile_base(self);
    void* p = self->field0;
    if (p)
        CActiveFile_free(p);
}