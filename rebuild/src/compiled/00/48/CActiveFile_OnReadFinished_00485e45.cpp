#pragma optimize("s",on)
struct CActiveFile {
    void* buf;
};

void __fastcall CActiveFile_Helper(CActiveFile* self);
void CActiveFile_Free(void* p);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_Helper(self);
    void* p = self->buf;
    if (p)
        CActiveFile_Free(p);
}