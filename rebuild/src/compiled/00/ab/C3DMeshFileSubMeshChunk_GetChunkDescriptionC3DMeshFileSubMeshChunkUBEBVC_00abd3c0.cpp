// C3DMeshFileSubMeshChunk::GetChunkDescription (virtual const) 0x00abd3c0
// Returns a CCharString by value, built from a fixed string literal with len=-1.

struct CCharString {
    char* m_pStr;
    int   m_len;
    CCharString(const char* s, int n);   // external ctor -> call 0x99ebf0
};

struct C3DMeshFileSubMeshChunk {
    void* vtbl;
    virtual CCharString GetChunkDescription() const;
};

CCharString C3DMeshFileSubMeshChunk::GetChunkDescription() const
{
    return CCharString("SubMesh", -1);
}