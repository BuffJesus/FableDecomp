// CActiveFile::OnReadFinished @ 0x004c5c60
// retail: push esi; mov esi,ecx; call base::OnReadFinished(this);
//         mov esi,[esi]; test esi,esi; jz end; push esi; call free_func(esi); add esp,4; end.

struct CActiveFile {
    void* p;   // +0
};

// base class read-finished handler (thiscall, this in ecx)
void __fastcall CActiveFile_BaseReadDone(void* self);

// cdecl free function taking the +0 pointer
void FreePtr(void* p);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_BaseReadDone(self);
    void* p = self->p;
    if (p)
        FreePtr(p);
}