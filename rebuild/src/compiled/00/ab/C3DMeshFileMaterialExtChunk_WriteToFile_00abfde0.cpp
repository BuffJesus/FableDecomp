struct CDataOutputStream { void WriteULONG(unsigned long value); };
struct CChunkedFileSaver;
struct C3DMeshFileMaterialExtChunk {
    unsigned char Base[0x10];
    unsigned long SelfIllumination;
    void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;
};
void C3DMeshFileMaterialExtChunk::WriteToFile(
    CDataOutputStream& stream, CChunkedFileSaver&) const {
    stream.WriteULONG(1);
    stream.WriteULONG(SelfIllumination);
}