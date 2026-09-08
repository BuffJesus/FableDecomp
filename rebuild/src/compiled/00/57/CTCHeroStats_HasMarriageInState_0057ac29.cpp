#pragma optimize("s",on)
enum EMarriageState { MARRIAGE_STATE_NONE = 0 };
struct CHeroMarriageInfo {
    unsigned long WifeUIDLow, WifeUIDHigh;
    unsigned long MaritalHomeUIDLow, MaritalHomeUIDHigh;
    EMarriageState State;
    bool GayMarriage;
};
struct CHeroMarriageNode {
    CHeroMarriageNode* Next;
    CHeroMarriageNode* Previous;
    CHeroMarriageInfo Value;
};
struct CHeroMarriageListRetail { CHeroMarriageNode* Head; };
struct CTCHeroStatsRetail {
    unsigned char Base[0x1d8];
    CHeroMarriageListRetail Marriages;
    bool HasMarriageInState(EMarriageState state) const;
};
bool CTCHeroStatsRetail::HasMarriageInState(EMarriageState state) const {
    CHeroMarriageNode* const sentinel = Marriages.Head;
    for (CHeroMarriageNode* node = sentinel->Next; node != sentinel; node = node->Next) {
        if (node->Value.State == state)
            return true;
    }
    return false;
}