// C3DMeshFileEngineMaterialChunk::GetChunkDescription
// retail 0x00ac6270 : const method returning CCharString by value.
// push -1 ; push <lit> ; ecx=retbuf ; call CCharString::ctor(char*, int) ; return retbuf

struct CCharString {
    char* m_data;
    // ctor is defined out-of-line in another TU (retail 0x0099ebf0); leaving it
    // undefined here forces the compiler to treat the call as opaque so the
    // return-buffer pointer is parked in a callee-saved register (esi) across it.
    CCharString(const char* s, int len);
};

struct C3DMeshFileEngineMaterialChunk {
    void* vtbl;
    CCharString GetChunkDescription() const;
};

CCharString C3DMeshFileEngineMaterialChunk::GetChunkDescription() const {
    return CCharString("Engine material chunk", -1);
}