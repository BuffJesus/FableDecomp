#pragma optimize("s",on)
struct CActiveFile {
    void* p; // +0x0
};

extern "C" void __fastcall CActiveFile_Helper(CActiveFile* self);
extern "C" void __cdecl CActiveFile_Release(void* obj);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_Helper(self);
    void* q = self->p;
    if (q != 0)
        CActiveFile_Release(q);
}