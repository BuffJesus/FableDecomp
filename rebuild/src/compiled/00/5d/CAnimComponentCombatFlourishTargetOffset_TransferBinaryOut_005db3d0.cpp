#pragma pack(push, 1)
struct C2DVector { float X; float Y; };
struct CDataOutputStream {
    void Write(bool value);
    void Write(const C2DVector& value);
};
struct CAnimComponentCombatFlourishTargetOffset {
    unsigned char Reserved[8];
    bool TargetOffset;
    C2DVector TargetOffsetVector;
    void TransferBinaryOut(CDataOutputStream& output) const;
};
#pragma pack(pop)
void CAnimComponentCombatFlourishTargetOffset::TransferBinaryOut(CDataOutputStream& output) const
{
    output.Write(TargetOffset);
    output.Write(TargetOffsetVector);
}