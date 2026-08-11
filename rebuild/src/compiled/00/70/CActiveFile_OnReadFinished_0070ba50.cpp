// CActiveFile::OnReadFinished @ 0x0070ba50
struct CActiveFile { void* ptr; };

extern void __fastcall base_OnReadFinished_impl(CActiveFile* self);
extern "C" void free_impl(void* p);

void __fastcall OnReadFinished(CActiveFile* self)
{
    base_OnReadFinished_impl(self);
    void* p = self->ptr;
    if (p)
        free_impl(p);
}