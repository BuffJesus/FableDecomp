struct CActiveFile {
    void* p;
    void* q;
};
extern "C" void __cdecl ExternFree(void* p);
void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    void* h = self->p;
    if (h)
        ExternFree(h);
}