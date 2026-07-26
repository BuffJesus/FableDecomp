struct CActiveFile {
    void* p; // +0x0
    void* q; // +0x4
};

extern void __fastcall CActiveFile_Base_OnReadFinished(CActiveFile* self);
extern void __cdecl FreeBuffer(void* p);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_Base_OnReadFinished(self);
    void* buf = self->p;
    if (buf)
        FreeBuffer(buf);
}