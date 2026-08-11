// CActiveFile::OnReadFinished  @ 009f9c20  __fastcall void(CActiveFile*)
// push esi; mov esi,ecx; call 0x9f9b30(this); esi=[this]; if(esi) free(esi); ret

void __cdecl ActiveFileFree(void* p);           // 0xbfea14 (cdecl free)

struct CActiveFile {
    void* buffer;   // +0x00
    void ProcessReadDone();  // 0x9f9b30 (thiscall member, no args, void)
    void OnReadFinished();
};

void CActiveFile::OnReadFinished()
{
    ProcessReadDone();
    void* p = buffer;
    if (p)
        ActiveFileFree(p);
}