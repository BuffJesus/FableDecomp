// CActiveFile::OnReadFinished @ 0x00aacd80
// void __fastcall(CActiveFile* this)

struct CActiveFile {
    void* field0;                        // [this+0]
};

// 0x00aacc60 : __fastcall member, no explicit args (ecx = this)
void __fastcall CActiveFile_PostReadStep(CActiveFile* self);
// 0x00bfea14 : cdecl free-like
void __cdecl FreeBlock(void* p);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_PostReadStep(self);
    void* p = self->field0;
    if (p)
        FreeBlock(p);
}