#pragma optimize("s",on)
// CActiveFile::OnReadFinished @ 00cd3211
// push esi; mov esi,ecx; call base(this); esi=*this; if(esi) free(esi); ret

struct CActiveFile;

extern "C" void __fastcall base_helper(CActiveFile* self);   // 0x00cd2ddc
extern "C" void __cdecl fable_free(void* p);                 // 0x00bfea14

struct CActiveFile {
    void* p0;
};

void __fastcall CActiveFile__OnReadFinished(CActiveFile* self)
{
    base_helper(self);
    void* q = self->p0;
    if (q)
        fable_free(q);
}