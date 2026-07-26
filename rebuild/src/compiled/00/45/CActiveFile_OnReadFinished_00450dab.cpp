#pragma optimize("s",on)
struct CActiveFile {
    void* p; // +0x0
    void* q; // +0x4
};

extern void __fastcall CActiveFile_Helper(CActiveFile* self);
extern void __cdecl CActiveFile_FreeBuf(void* buf);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_Helper(self);
    void* b = self->p;
    if (b)
        CActiveFile_FreeBuf(b);
}