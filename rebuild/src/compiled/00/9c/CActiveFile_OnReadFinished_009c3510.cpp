// CActiveFile::OnReadFinished @ 009c3510
// push esi; mov esi,ecx; mov eax,[esi]; cmp eax,-1; je end;
// push eax; call [0x143fe30]; mov [esi],-1; end: pop esi; ret

typedef int (__stdcall *CloseFn)(void*);

// IAT slot at 0x143fe30 (CloseHandle)
CloseFn * const g_pCloseSlot = (CloseFn*)0x0143fe30;

struct CActiveFile
{
    void* handle;   // +0x00
    void OnReadFinished();
};

void CActiveFile::OnReadFinished()
{
    if (this->handle != (void*)-1)
    {
        (*g_pCloseSlot)(this->handle);
        this->handle = (void*)-1;
    }
}