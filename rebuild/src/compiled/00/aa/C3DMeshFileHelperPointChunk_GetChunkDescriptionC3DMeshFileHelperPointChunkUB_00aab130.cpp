// C3DMeshFileHelperPointChunk::GetChunkDescription
// Byte-exact reconstruction of retail 0x00aab130.
//
// Retail body (NRVO struct-return forwarder):
//   push esi
//   mov  esi, [esp+8]        ; hidden return-buffer ptr
//   push -1
//   push 0x129e5e0           ; string literal
//   mov  ecx, esi
//   call CCharString::CCharString(const char*, int)
//   mov  eax, esi
//   pop  esi
//   ret  4

// CCharString: a small string class. Only the ctor(const char*, int)
// signature/ABI matters for byte parity; body is a link-only stub.
class CCharString {
public:
    CCharString(const char* s, int len);
private:
    char* m_p;
    int   m_len;
    int   m_cap;
};

// Constructor is only DECLARED here (defined in another TU) so the compiler
// must emit a real, opaque __fastcall call, matching retail's
// `call CCharString::CCharString` and forcing the ret-buffer ptr to live in a
// callee-saved register across the call.

class C3DMeshFileHelperPointChunk {
public:
    virtual const CCharString GetChunkDescription() const;
};

const CCharString C3DMeshFileHelperPointChunk::GetChunkDescription() const {
    return CCharString("PointChunk", -1);
}