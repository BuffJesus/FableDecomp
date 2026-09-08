struct CMatrix3x4 { float Elements[12]; };
struct CDataOutputStream { void Write(const CMatrix3x4& value); };
struct CChunkedFileSaver;
struct C3DMeshFileChunk {
    void WriteToFile(CDataOutputStream& output, CChunkedFileSaver& saver) const;
};
// Retail places Transform at +0x2c; Ego_r places the same PDB member at
// +0x34 because the debug-build base subobject is eight bytes larger.
struct C3DMeshFileTransformationChunkRetail : C3DMeshFileChunk {
    unsigned char Reserved[0x2c];
    CMatrix3x4 Transform;
    void WriteToFile(CDataOutputStream& output, CChunkedFileSaver& saver) const;
};
void C3DMeshFileTransformationChunkRetail::WriteToFile(CDataOutputStream& output, CChunkedFileSaver& saver) const
{
    output.Write(Transform);
    C3DMeshFileChunk::WriteToFile(output, saver);
}