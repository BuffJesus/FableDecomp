#pragma optimize("s",on)
struct CActiveFile {
    void* buf;
};

extern void __fastcall CActiveFile_Notify(CActiveFile* self);
extern void __cdecl CActiveFile_Free(void* p);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_Notify(self);
    void* p = self->buf;
    if (p)
        CActiveFile_Free(p);
}