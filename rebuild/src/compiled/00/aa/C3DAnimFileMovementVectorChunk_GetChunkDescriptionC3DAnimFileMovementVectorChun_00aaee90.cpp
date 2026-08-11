// C3DAnimFileMovementVectorChunk::GetChunkDescription  @ 0x00aaee90
// const virtual returning CCharString by value.

class CCharString {
public:
    CCharString(const char* s, int n);
    void* p;
};

struct C3DAnimFileMovementVectorChunk {
    virtual ~C3DAnimFileMovementVectorChunk();
    virtual CCharString GetChunkDescription() const;
};

CCharString C3DAnimFileMovementVectorChunk::GetChunkDescription() const {
    return CCharString("MovementVector", -1);
}