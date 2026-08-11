#pragma optimize("s",on)
// CActiveFile::OnReadFinished @ 0x00477bfc
// __fastcall void OnReadFinished(CActiveFile* this)

struct CActiveFile {
    void* stream;   // +0x00
    int   status;   // +0x04
};

// __fastcall helper at 0x4774b9: ecx = stream (this), edx = status arg
void __fastcall CActiveFile_Notify(void* stream, int status);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_Notify(self->stream, self->status);
    if (self->stream)
        ::operator delete(self->stream);
}