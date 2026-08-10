#pragma optimize("s",on)

struct CActiveFile {
    void* buf;
};

extern "C" void __fastcall Helper_OnRead(CActiveFile* self);
extern "C" void __cdecl retail_free(void* p);

void __fastcall OnReadFinished(CActiveFile* self)
{
    Helper_OnRead(self);
    void* p = self->buf;
    if (p)
        retail_free(p);
}