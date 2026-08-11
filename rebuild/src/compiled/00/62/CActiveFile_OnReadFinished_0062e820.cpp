// CActiveFile::OnReadFinished @ 0062e820
struct CActiveFile;

void __fastcall base_helper(CActiveFile* self);       // 0x62e2b0, ecx=this
void __cdecl free_helper(void* p);                    // 0xbfea14, cdecl

struct CActiveFile {
    void* m0;   // [this+0]
};

void __fastcall OnReadFinished(CActiveFile* self)
{
    base_helper(self);
    void* p = self->m0;
    if (p)
        free_helper(p);
}