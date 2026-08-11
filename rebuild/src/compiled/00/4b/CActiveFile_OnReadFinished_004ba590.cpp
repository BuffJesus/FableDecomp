// CActiveFile::OnReadFinished @ 004ba590
struct CActiveFile;

// base-class read-complete handler (thiscall, this in ecx), base 0x4b8c00
void __fastcall BaseReadDone(CActiveFile* self);
// free cdecl helper, free 0xbfea14
void FreeHelper(void* p);

struct CActiveFile {
    void* firstMember;
};

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    BaseReadDone(self);
    void* p = self->firstMember;
    if (p != 0)
        FreeHelper(p);
}