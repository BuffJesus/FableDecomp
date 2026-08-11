// CTCInventoryAbilities::AddLearnedExpression @ 0x00c222b0
// void __fastcall CTCInventoryAbilities::AddLearnedExpression(long value)
//   -> (subobject at this+0x318).Insert(&value)
// value is a by-value parameter whose address is passed to Insert.

struct LearnedSet {
    void Insert(long* p);
};

struct CTCInventoryAbilities {
    char pad[0x318];
    LearnedSet learned;   // at +0x318

    void AddLearnedExpression(long value);
};

void CTCInventoryAbilities::AddLearnedExpression(long value)
{
    long expr = value;
    learned.Insert(&expr);
}