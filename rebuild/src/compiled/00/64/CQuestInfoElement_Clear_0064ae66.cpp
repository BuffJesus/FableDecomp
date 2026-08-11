#pragma optimize("s",on)
// Byte-exact reconstruction of CQuestInfoElement::Clear @ 0x0064ae66 (VC7.1 /O2 /Oy)
//
// Retail disasm:
//   and dword ptr [ecx+0x60], 0     ; m_field60 = 0
//   push esi
//   lea esi, [ecx+0x68]             ; sub = this->m_list (embedded at +0x68)
//   jmp cond
// body:
//   mov ecx, esi
//   call 0x655fe9                   ; sub->PopBack()  (non-virtual member)
// cond:
//   mov eax, [esi+0x10]
//   cmp eax, [esi]
//   jne body
//   pop esi
//   ret
//
// The +0x68 subobject is a small container. Layout used by the loop:
//   +0x00  begin pointer
//   +0x10  a pointer compared against begin; loop pops until [+0x10] == [+0x00]

struct SubList
{
    void* m_00;   // +0x00
    void* m_04;   // +0x04
    void* m_08;   // +0x08
    void* m_0c;   // +0x0c
    void* m_10;   // +0x10
    // non-virtual member; emitted as a direct call
    void PopEntry();
};

struct CQuestInfoElement
{
    char     m_pad[0x60];   // +0x00 .. +0x5f
    int      m_field60;     // +0x60
    char     m_pad2[0x04];  // +0x64
    SubList  m_list;        // +0x68
    void Clear();
};

void CQuestInfoElement::Clear()
{
    m_field60 = 0;
    SubList* sub = &m_list;
    while (sub->m_10 != sub->m_00)
        sub->PopEntry();
}