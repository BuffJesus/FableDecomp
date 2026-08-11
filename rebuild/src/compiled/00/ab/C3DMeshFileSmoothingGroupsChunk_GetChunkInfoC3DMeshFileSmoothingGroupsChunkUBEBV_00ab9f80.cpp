// Faithful VC7.1 reconstruction of
// C3DMeshFileSmoothingGroupsChunk::GetChunkInfo (retail 0x00ab9f80)
//
// Retail disasm:
//   push esi
//   mov  esi, [esp+8]        ; esi = hidden return-buffer ptr
//   push -1                  ; len = -1
//   push 0x122d70e           ; const char* literal
//   mov  ecx, esi            ; this = return buffer
//   call CCharString::CCharString(const char*, long)  (0x99ebf0)
//   mov  eax, esi            ; return the buffer ptr
//   pop  esi
//   ret  4
//
// => returns CCharString("<chunk-name>", -1) by value.

class CCharString
{
public:
    // (const char*, long) ctor at 0x99ebf0
    CCharString(const char* str, long len);
private:
    char* m_pData;
};

class C3DMeshFileSmoothingGroupsChunk
{
public:
    virtual CCharString GetChunkInfo() const;
};

CCharString C3DMeshFileSmoothingGroupsChunk::GetChunkInfo() const
{
    return CCharString("SmoothingGroups", -1);
}