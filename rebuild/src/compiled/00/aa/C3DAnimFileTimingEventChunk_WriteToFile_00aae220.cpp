struct CCharString { void* Data; };
struct CDataOutputStream {
    void Write(const CCharString& value);
    void Write(float value);
};
struct CChunkedFileSaver;
struct C3DAnimFileTimingEventChunk {
    unsigned char Reserved[0x10];
    CCharString Name;
    float Time;
    void WriteToFile(CDataOutputStream& output, CChunkedFileSaver& saver) const;
};
void C3DAnimFileTimingEventChunk::WriteToFile(CDataOutputStream& output, CChunkedFileSaver&) const
{
    output.Write(Name);
    output.Write(Time);
}