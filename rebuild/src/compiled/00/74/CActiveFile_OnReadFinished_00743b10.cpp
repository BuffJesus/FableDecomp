// CActiveFile::OnReadFinished @ 0x00743b10
struct CActiveFile {
    void* member0;
};

extern "C" void __fastcall CActiveFile_base(CActiveFile* self); // 0x742290
extern "C" void free_func(void* p);                             // 0xbfea14

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_base(self);
    void* m = self->member0;
    if (m) {
        free_func(m);
    }
}