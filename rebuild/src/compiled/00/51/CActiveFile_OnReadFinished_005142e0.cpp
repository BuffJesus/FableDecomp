// CActiveFile::OnReadFinished @ 005142e0
// 56 8bf1 e8.. 8b36 85f6 7409 56 e8.. 83c404 5e c3
//   push esi; mov esi,ecx; call base(this); mov esi,[esi];
//   test esi,esi; je +; push esi; call free; add esp,4; pop esi; ret

void my_free(void*);

struct CActiveFile;

// base method @ 0x510d40, __fastcall(this)
void __fastcall CActiveFile_base(CActiveFile* self);

struct CActiveFile {
    void* buffer; // +0
};

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_base(self);
    void* p = self->buffer;
    if (p)
        my_free(p);
}