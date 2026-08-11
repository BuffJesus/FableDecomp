#include <stdio.h>

struct LearnedSet {
    long captured;
    long capturedVal;
    void Insert(long* p);
};

struct CTCInventoryAbilities {
    char pad[0x318];
    LearnedSet learned;
    void AddLearnedExpression(long value);
};

void CTCInventoryAbilities::AddLearnedExpression(long value)
{
    long expr = value;
    learned.Insert(&expr);
}

void LearnedSet::Insert(long* p)
{
    captured = 1;
    capturedVal = *p;
}

int main()
{
    static CTCInventoryAbilities obj;
    obj.learned.captured = 0;
    obj.learned.capturedVal = 0;

    obj.AddLearnedExpression(0x1234abcd);

    if (obj.learned.captured == 1 && obj.learned.capturedVal == 0x1234abcd)
        printf("PASS_ALE_OK\n");
    else
        printf("FAIL val=%08lx\n", obj.learned.capturedVal);
    return 0;
}