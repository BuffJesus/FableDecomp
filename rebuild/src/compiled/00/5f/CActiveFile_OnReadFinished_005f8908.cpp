#pragma optimize("s",on)

struct CActiveFile {
    void* buf;
};

extern "C" void __fastcall CActiveFile_PreRead(CActiveFile* self);
extern "C" void __cdecl retail_free(void* p);

void __fastcall OnReadFinished(CActiveFile* self)
{
    CActiveFile_PreRead(self);
    void* p = self->buf;
    if (p)
        retail_free(p);
}