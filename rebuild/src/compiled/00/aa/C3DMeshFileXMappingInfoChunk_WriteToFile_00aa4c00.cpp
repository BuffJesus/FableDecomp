struct CDataOutputStream { void WriteULONG(unsigned long value); void WriteFloat(float value); };
struct CChunkedFileSaver;
struct C3DMeshFileXMappingInfoChunk {
    unsigned char Base[0x10];
    float AverageTextureStretch;
    void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;
};
void C3DMeshFileXMappingInfoChunk::WriteToFile(CDataOutputStream& stream, CChunkedFileSaver&) const {
    stream.WriteULONG(1);
    stream.WriteFloat(AverageTextureStretch);
}