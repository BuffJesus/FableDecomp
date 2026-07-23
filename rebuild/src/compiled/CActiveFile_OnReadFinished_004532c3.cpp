#pragma optimize("s",on)
struct CActiveFile {
    void* p0; // +0x0
    void* p1; // +0x4
};

extern void __fastcall Handler(void* a, void* b, char* c);
extern void __cdecl FreeMem(void* p);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    char local;
    Handler(self->p0, self->p1, &local);
    if (self->p0)
        FreeMem(self->p0);
}