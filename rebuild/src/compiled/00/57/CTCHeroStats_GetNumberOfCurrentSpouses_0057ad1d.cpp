#pragma optimize("s",on)

#include "engine/CHeroMarriageInfo.h"

enum EMarriageState {
    MARRIAGE_STATE_NONE = 0,
    MARRIAGE_STATE_1 = 1,
    MARRIAGE_STATE_2 = 2,
    MARRIAGE_STATE_3 = 3,
    MARRIAGE_STATE_4 = 4
};

struct CHeroMarriageListNode {
    CHeroMarriageListNode* Next;
    CHeroMarriageListNode* Previous;
    CHeroMarriageInfo Marriage;
};

struct CHeroMarriageList {
    CHeroMarriageListNode* Head;
};

// Retail omits 0x18 bytes that precede Marriages in the Ego_r layout.
struct CTCHeroStatsRetail {
    unsigned char EarlierFields[0x1d8];
    CHeroMarriageList Marriages;

    long GetNumberOfCurrentSpouses() const;
};

static EMarriageState GetMarriageState(const CHeroMarriageInfo& marriage)
{
    return *reinterpret_cast<const EMarriageState*>(marriage.State);
}

long CTCHeroStatsRetail::GetNumberOfCurrentSpouses() const
{
    long count = 0;
    CHeroMarriageListNode* const head = Marriages.Head;

    for (CHeroMarriageListNode* node = head->Next;
         node != head;
         node = node->Next) {
        const EMarriageState state = GetMarriageState(node->Marriage);
        if (state == MARRIAGE_STATE_1 ||
            state == MARRIAGE_STATE_3 ||
            state == MARRIAGE_STATE_2 ||
            state == MARRIAGE_STATE_4) {
            ++count;
        }
    }

    return count;
}
