// CActiveFile::OnReadFinished @ 00bf3140
struct CActiveFile {
    void* pReader; // +0
};

extern void __stdcall CActiveFile_sub_bf29e0(void* p);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    void* p = self->pReader;
    if (p) {
        CActiveFile_sub_bf29e0(p);
        self->pReader = 0;
    }
}