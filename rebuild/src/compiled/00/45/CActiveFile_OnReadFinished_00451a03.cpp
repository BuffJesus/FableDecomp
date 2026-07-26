#pragma optimize("s",on)
struct CActiveFile;
extern "C" void __fastcall CActiveFile_helper(CActiveFile* self);
extern "C" void __cdecl CActiveFile_release(void* p);

struct CActiveFile {
    void* field0;
};

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_helper(self);
    void* p = self->field0;
    if (p) {
        CActiveFile_release(p);
    }
}