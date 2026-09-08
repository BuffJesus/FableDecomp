// Owner and virtual signature are from the original PDB symbol. Retail's
// canonical chunk-description string is retained as a named external datum.
struct CCharString {
    int Data;
    CCharString(const char* text, int length);
};

extern char kC3DMeshFileMapChunkDescription[];

struct C3DMeshFileMapChunk {
    CCharString GetChunkDescription() const;
};

CCharString C3DMeshFileMapChunk::GetChunkDescription() const {
    return CCharString(kC3DMeshFileMapChunkDescription, -1);
}