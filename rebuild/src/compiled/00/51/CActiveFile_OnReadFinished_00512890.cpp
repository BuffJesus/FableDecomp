// CActiveFile::OnReadFinished @ 00512890
// shape: base_510d40(this); if(*this) free_bfea14(*this);

struct CActiveFile {
    void* p;   // *this at +0
};

extern "C" void __fastcall base_510d40(CActiveFile* self);   // 00510d40 thiscall
extern "C" void __cdecl free_bfea14(void* p);                // 00bfea14 cdecl

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    base_510d40(self);
    void* q = self->p;
    if (q)
        free_bfea14(q);
}