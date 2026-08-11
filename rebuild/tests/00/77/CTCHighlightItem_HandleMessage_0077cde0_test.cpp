#include <stdio.h>

static int g_actionCalls = 0;
static int g_typeToReturn = 0;

struct CMessageBase {
    virtual void vf0() {}
    virtual int GetType() { return g_typeToReturn; }
};

struct CTCHighlightItem {
    void DoHighlightAction();
    void HandleMessage(CMessageBase* msg);
};

void CTCHighlightItem::DoHighlightAction()
{
    g_actionCalls++;
}

void CTCHighlightItem::HandleMessage(CMessageBase* msg)
{
    if (msg->GetType() == 2)
        this->DoHighlightAction();
}

int main()
{
    CTCHighlightItem item;
    CMessageBase msg;

    g_typeToReturn = 1;
    g_actionCalls = 0;
    item.HandleMessage(&msg);
    if (g_actionCalls != 0) { printf("FAIL type1\n"); return 1; }

    g_typeToReturn = 2;
    g_actionCalls = 0;
    item.HandleMessage(&msg);
    if (g_actionCalls != 1) { printf("FAIL type2\n"); return 1; }

    g_typeToReturn = 3;
    g_actionCalls = 0;
    item.HandleMessage(&msg);
    if (g_actionCalls != 0) { printf("FAIL type3\n"); return 1; }

    printf("HIGHLIGHT_HANDLEMSG_OK\n");
    return 0;
}