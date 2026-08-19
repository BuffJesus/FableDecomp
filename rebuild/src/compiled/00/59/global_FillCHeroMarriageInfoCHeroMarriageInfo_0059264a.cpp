#pragma optimize("s",on)
// std::fill over CHeroMarriageInfo (POD size 0x1c = 28 bytes = 7 dwords)
struct CHeroMarriageInfo {
    unsigned long w[7];
};

// __fastcall: ecx=_First, edx=_Last, stack=_Val
void __fastcall Fill_CHeroMarriageInfo(CHeroMarriageInfo *_First, CHeroMarriageInfo *_Last, const CHeroMarriageInfo *_Val)
{
    for (; _First != _Last; ++_First)
        *_First = *_Val;
}