struct CDataOutputStream { void WriteULONG(unsigned long value); void WriteFloat(float value); };
struct CChunkedFileSaver;
struct C3DAnimFileXAllocationSizeChunk {
    unsigned char Base[0x10];
    unsigned long AllocSize;
    void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;
};
void C3DAnimFileXAllocationSizeChunk::WriteToFile(CDataOutputStream& stream, CChunkedFileSaver&) const {
    stream.WriteULONG(1);
    stream.WriteULONG(AllocSize);
}