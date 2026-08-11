// Byte-exact reconstruction of
// C3DMeshFileXAnimatedPrimitiveChunk::GetChunkDescription  @ 0x00aa7090
//
// Retail:
//   push esi
//   mov  esi, [esp+8]          ; hidden return-buffer ptr
//   push -1
//   push 0x129e2a4             ; const char* literal
//   mov  ecx, esi
//   call 0x99ebf0             ; CCharString::CCharString(const char*, int)
//   mov  eax, esi
//   pop  esi
//   ret  4
//
// It is a const virtual returning CCharString by value; the body just
// constructs a CCharString from a fixed string literal address with len -1.

class CCharString
{
public:
    // ctor at 0x99ebf0 : (const char* str, int len)
    CCharString(const char* str, int len);
private:
    char* m_ptr;
};

class C3DMeshFileXAnimatedPrimitiveChunk
{
public:
    virtual CCharString GetChunkDescription() const;
    // pad so the slot doesn't matter for byte parity of the body itself
};

CCharString C3DMeshFileXAnimatedPrimitiveChunk::GetChunkDescription() const
{
    return CCharString((const char*)0x129e2a4, -1);
}