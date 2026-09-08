struct CDataOutputStream { void Write(float value); };
struct CReactionFrequencyTraits_LinearTime {
    unsigned char Reserved[4];
    float MinWait;
    float WaitRange;
    unsigned long FrameHappened;
    void TransferBinaryOut(CDataOutputStream& output) const;
};
void CReactionFrequencyTraits_LinearTime::TransferBinaryOut(CDataOutputStream& output) const
{
    output.Write(MinWait);
    output.Write(WaitRange);
}