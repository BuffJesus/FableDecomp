#pragma optimize("s",on)
extern "C" void free(void*);
struct CActiveFile {
    void* buf;
};
void __fastcall helper_read(CActiveFile* self);
void __fastcall OnReadFinished_3(CActiveFile* self)
{
    helper_read(self);
    void* p = self->buf;
    if (p)
        free(p);
}