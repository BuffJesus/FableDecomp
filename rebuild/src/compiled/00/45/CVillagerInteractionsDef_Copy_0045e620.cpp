#pragma optimize("s",on)
struct CDefClassBase { void Copy(const CDefClassBase* source); };
struct CDefString { unsigned long Value; };
// Retail omits one debug-layout base dword, placing the PDB fields at +0x3c.
struct CVillagerInteractionsDefRetail : CDefClassBase {
    unsigned char Reserved[0x3c];
    CDefString Animation1;
    CDefString Animation2;
    float Distance;
    void Copy(const CDefClassBase* source);
};
void CVillagerInteractionsDefRetail::Copy(const CDefClassBase* sourceBase)
{
    CDefClassBase::Copy(sourceBase);
    const CVillagerInteractionsDefRetail* source = (const CVillagerInteractionsDefRetail*)sourceBase;
    Animation1 = source->Animation1;
    Animation2 = source->Animation2;
    Distance = source->Distance;
}