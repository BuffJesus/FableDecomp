// CCharString: models the engine string. Only the (const char*, long) ctor
// is defined here (out-of-line so it becomes the external call 0x99ebf0).
class CCharString {
public:
    CCharString(const char* str, long len);
private:
    char* m_data;
};

// The class whose const method returns the chunk description by value.
// Enough virtuals so GetChunkDescription is a real virtual member; the
// engine method itself never touches 'this' (returns a constant string).
class C3DMeshFileGroupChunk {
public:
    virtual CCharString GetChunkDescription() const;
};

CCharString C3DMeshFileGroupChunk::GetChunkDescription() const {
    return CCharString("group", -1);
}