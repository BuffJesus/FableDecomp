// Owner and virtual signature are from the original PDB symbol. Retail's
// canonical chunk-description string is retained as a named external datum.
struct CCharString {
    int Data;
    CCharString(const char* text, int length);
};

extern char kC3DAnimFileTimingEventChunkDescription[];

struct C3DAnimFileTimingEventChunk {
    CCharString GetChunkDescription() const;
};

CCharString C3DAnimFileTimingEventChunk::GetChunkDescription() const {
    return CCharString(kC3DAnimFileTimingEventChunkDescription, -1);
}