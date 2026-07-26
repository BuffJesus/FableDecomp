#pragma optimize("s",on)
struct CActiveFile { void* p; };
extern "C" void __fastcall CActiveFile_BaseNotify(CActiveFile* self);
extern "C" void FreeBuffer(void* p);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_BaseNotify(self);
    void* buf = self->p;
    if (buf)
        FreeBuffer(buf);
}