#include "engine/CTCHighlightItem.h"  // retyped onto the PDB layout; byte parity re-verified
struct CMessageBase {
    virtual void vf0();
    virtual int GetType();
};

struct CTCHighlightItem_Methods : CTCHighlightItem {
    void DoHighlightAction();
    void HandleMessage(CMessageBase* msg);
};

void CTCHighlightItem_Methods::HandleMessage(CMessageBase* msg)
{
    if (msg->GetType() == 2)
        this->DoHighlightAction();
}