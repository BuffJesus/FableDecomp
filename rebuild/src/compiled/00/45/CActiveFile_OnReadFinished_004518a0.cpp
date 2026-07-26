#pragma optimize("s",on)
struct CActiveFile { void* buf; void Close(); };
extern void FreeBuffer(void* p);
void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    self->Close();
    void* p = self->buf;
    if (p != 0)
        FreeBuffer(p);
}