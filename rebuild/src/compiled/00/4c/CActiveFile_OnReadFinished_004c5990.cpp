// CActiveFile::OnReadFinished @ 004c5990
struct CActiveFile {
    void* buf;   // +0x00
};

// base method called with ecx=this (__fastcall modeled as __fastcall)
void __fastcall CActiveFile_base(CActiveFile* self);
// cdecl free-like function
void free_bfea14(void* p);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_base(self);
    void* p = self->buf;
    if (p)
        free_bfea14(p);
}