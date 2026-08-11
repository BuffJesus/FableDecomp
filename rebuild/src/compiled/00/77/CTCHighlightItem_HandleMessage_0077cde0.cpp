struct CMessageBase {
    virtual void vf0();
    virtual int GetType();
};

struct CTCHighlightItem {
    void DoHighlightAction();
    void HandleMessage(CMessageBase* msg);
};

void CTCHighlightItem::HandleMessage(CMessageBase* msg)
{
    if (msg->GetType() == 2)
        this->DoHighlightAction();
}