// CActiveFile::OnReadFinished @ 00835390
// __fastcall void OnReadFinished(CActiveFile* this)

void __fastcall CActiveFile_BaseOnReadFinished(void* self);  // 0x834f90
void  __cdecl   CActiveFile_FreeBuffer(void* p);             // 0xbfea14

struct CActiveFile {
    void* buffer;  // +0x00
};

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_BaseOnReadFinished(self);
    void* p = self->buffer;
    if (p)
        CActiveFile_FreeBuffer(p);
}