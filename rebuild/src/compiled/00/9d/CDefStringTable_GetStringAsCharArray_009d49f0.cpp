// CDefStringTable::GetStringAsCharArray(long index) -- 0x009d49f0
// char * __fastcall(CDefStringTable *this, long index)
//
// Layout evidence from bytes:
//   push esi
//   mov esi, [esp+8]          ; esi = index (arg1, thiscall stack slot after ret addr)
//   cmp esi, -1
//   jnz L_lookup
//   mov eax, 0x0122d70e       ; shared global empty-string byte
//   pop esi
//   ret 4
// L_lookup:
//   add ecx, 0x0c             ; ecx = &this->m_stream (CMemoryDataOutputStream member at +0xc)
//   call CMemoryDataOutputStream::PeekData
//   lea eax, [eax+esi+4]      ; eax = (char*)base + index + 4
//   pop esi
//   ret 4

extern "C" char DAT_0122d70e;

class CMemoryDataOutputStream
{
public:
    void *PeekData();
};

class CDefStringTable
{
public:
    char pad0[0xc];
    CMemoryDataOutputStream m_stream;

    char *GetStringAsCharArray(long index);
};

char *CDefStringTable::GetStringAsCharArray(long index)
{
    if (index == -1)
    {
        return &DAT_0122d70e;
    }

    void *base = m_stream.PeekData();
    return (char *)base + index + 4;
}