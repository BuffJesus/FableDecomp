// Owner and virtual signature are from the original PDB symbol. Retail's
// canonical chunk-description string is retained as a named external datum.
struct CCharString {
    int Data;
    CCharString(const char* text, int length);
};

extern char kC3DAnimFileSequenceChunkDescription[];

struct C3DAnimFileSequenceChunk {
    CCharString GetChunkDescription() const;
};

CCharString C3DAnimFileSequenceChunk::GetChunkDescription() const {
    return CCharString(kC3DAnimFileSequenceChunkDescription, -1);
}