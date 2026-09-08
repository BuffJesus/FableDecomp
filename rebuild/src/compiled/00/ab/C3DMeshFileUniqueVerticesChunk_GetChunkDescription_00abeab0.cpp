// Owner and virtual signature are from the original PDB symbol. Retail's
// canonical chunk-description string is retained as a named external datum.
struct CCharString {
    int Data;
    CCharString(const char* text, int length);
};

extern char kC3DMeshFileUniqueVerticesChunkDescription[];

struct C3DMeshFileUniqueVerticesChunk {
    CCharString GetChunkDescription() const;
};

CCharString C3DMeshFileUniqueVerticesChunk::GetChunkDescription() const {
    return CCharString(kC3DMeshFileUniqueVerticesChunkDescription, -1);
}