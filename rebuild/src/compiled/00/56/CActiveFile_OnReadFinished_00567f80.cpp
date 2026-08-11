// CActiveFile::OnReadFinished @ 0x00567f80
struct CActiveFile { void* m0; };

extern void __fastcall CActiveFile_FrameUpdate_566dc0(CActiveFile* self); // thiscall this->ecx, void
extern void __cdecl re_free(void* p);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_FrameUpdate_566dc0(self);
    void* p = self->m0;
    if (p)
        re_free(p);
}