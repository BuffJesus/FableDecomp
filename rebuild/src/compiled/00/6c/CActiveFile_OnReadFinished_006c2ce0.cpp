// CActiveFile::OnReadFinished @ 006c2ce0
// 56              push esi
// 8bf1            mov esi, ecx
// e818ffffff      call 0x6c2c00  (base OnReadFinished, __fastcall this in ecx)
// 8b36            mov esi, [esi]  (member at +0)
// 85f6            test esi, esi
// 7409            jz skip
// 56              push esi
// e820bd5300      call 0xbfea14  (free func, cdecl arg)
// 83c404          add esp, 4
// 5e              pop esi
// c3              ret

struct CActiveFile {
    void* m_ptr; // +0
};

void __fastcall BaseOnReadFinished(CActiveFile* self);
extern "C" void FreeReleased(void* p);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    BaseOnReadFinished(self);
    void* p = self->m_ptr;
    if (p)
        FreeReleased(p);
}