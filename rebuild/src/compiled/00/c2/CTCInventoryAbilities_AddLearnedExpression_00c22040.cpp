// CTCInventoryAbilities::AddLearnedExpression @ 0x00c22040
// __fastcall void AddLearnedExpression(this, long value)
// forwards to a __fastcall method on the subobject at this+0x1f8,
// passing the value BY REFERENCE (address of the local param slot).

struct SubObj1F8 {
    void Add(long &value);   // __fastcall; receives &value as a stack arg
};

struct CTCInventoryAbilities {
    char pad0[0x1f8];
    SubObj1F8 sub;           // subobject at offset 0x1f8
    void AddLearnedExpression(long value);
};

void CTCInventoryAbilities::AddLearnedExpression(long value)
{
    long local = value;
    sub.Add(local);
}