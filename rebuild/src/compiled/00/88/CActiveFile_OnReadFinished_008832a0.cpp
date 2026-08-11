// CActiveFile::OnReadFinished  @ 0x008832a0
// push esi; mov esi,ecx; call helper (thiscall, no args); mov esi,[esi];
// test esi,esi; je skip; push esi; call free (cdecl); add esp,4; skip: pop esi; ret

void __fastcall CActiveFile_Helper(void* self);   // 0x882b80 (thiscall, no args)
void  the_free(void* p);                           // 0xbfea14 (cdecl, one arg)

struct CActiveFile {
    void* m_buffer;   // +0x00
};

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_Helper(self);
    void* p = self->m_buffer;
    if (p)
        the_free(p);
}