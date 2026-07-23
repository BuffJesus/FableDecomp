struct CHeroMarriageInfo {
    int a, b, c, d, e, f, g;
};

void __fastcall Fill(CHeroMarriageInfo* _First, CHeroMarriageInfo* _Last, const CHeroMarriageInfo& _Val)
{
    for (; _First != _Last; ++_First)
        *_First = _Val;
}