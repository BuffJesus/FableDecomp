// CVolume::ReleaseVolume  0x00a67460
// void __fastcall CVolume::ReleaseVolume(CVolume* this, <4-byte stack arg>)
//
// Disasm:
//   push esi; mov esi,ecx                     ; this -> esi
//   mov eax,[esi+4]; test eax,eax; je done
//   mov ecx,[eax]                             ; ecx = obj->vptr (table at obj+0)
//   push eax; call [ecx+8]                    ; table[2](obj), callee pops arg (__stdcall)
//   mov [esi+4],0
// done: pop esi; ret 4                         ; pops one 4-byte stack param

struct IRefObject;

struct IVTable
{
    // slot 2 == [vptr+8]; the object is passed as an explicit stack arg,
    // and the callee cleans it up (no caller add esp) => __stdcall
    void (__stdcall *slot0)(IRefObject* self);
    void (__stdcall *slot1)(IRefObject* self);
    void (__stdcall *slot2)(IRefObject* self);
};

struct IRefObject
{
    IVTable* vptr;   // +0 : pointer to the function table
};

struct CVolume
{
    void*       m_pad0;     // +0
    IRefObject* m_pObject;  // +4

    // one trailing 4-byte stack param (ignored) => callee epilogue emits ret 4
    void ReleaseVolume(int /*unused*/);
};

void CVolume::ReleaseVolume(int)
{
    IRefObject* p = m_pObject;
    if (p != 0)
    {
        p->vptr->slot2(p);
        m_pObject = 0;
    }
}