#pragma optimize("s",on)
struct CActiveFile {
    void* p; // +0x0
    void* q; // +0x4
};

extern void __fastcall Reader_Read(void* ecx, void* edx, unsigned char* buf);
extern void __cdecl FreeThing(void* p);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    unsigned char b;
    Reader_Read(self->p, self->q, &b);
    if (self->p) {
        FreeThing(self->p);
    }
}