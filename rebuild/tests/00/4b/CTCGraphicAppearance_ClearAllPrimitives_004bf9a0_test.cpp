
#include <cstdio>

static int g_slotCalls = 0;

struct Prim {
    virtual void slot0() { }
    virtual void slot1() { ++g_slotCalls; }
};

struct ListNode {
    ListNode* next;
    ListNode* prev;
    Prim* obj;
};

static int g_clearCalls = 0;
static int g_counterVal = 41;

struct MemberAt38 {
    int dummy;
    void Clear() { ++g_clearCalls; }
};

int GetSomeCounter() { return g_counterVal; }

struct CTCGraphicAppearance {
    char pad[0x38];
    MemberAt38 m38;
    ListNode* head;
    char pad2[0x54 - 0x40];
    int counter;
};

void __fastcall CTCGraphicAppearance_ClearAllPrimitives(CTCGraphicAppearance* self)
{
    ListNode* node = self->head->next;
    while (node != self->head) {
        node->obj->slot1();
        node = node->next;
    }
    self->m38.Clear();
    self->counter = GetSomeCounter() + 1;
}

int main() {
    CTCGraphicAppearance app;
    for (int i = 0; i < (int)sizeof(app); ++i) ((char*)&app)[i] = 0;

    ListNode sentinel;
    Prim p0, p1, p2;
    ListNode n0, n1, n2;
    n0.obj = &p0; n1.obj = &p1; n2.obj = &p2;

    // circular doubly linked list: sentinel -> n0 -> n1 -> n2 -> sentinel
    sentinel.next = &n0; n0.prev = &sentinel;
    n0.next = &n1;       n1.prev = &n0;
    n1.next = &n2;       n2.prev = &n1;
    n2.next = &sentinel; sentinel.prev = &n2;

    app.head = &sentinel;

    CTCGraphicAppearance_ClearAllPrimitives(&app);

    if (g_slotCalls == 3 && g_clearCalls == 1 && app.counter == 42) {
        std::printf("CTCGraphicAppearance_004bf9a0_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL slot=%d clear=%d counter=%d\n", g_slotCalls, g_clearCalls, app.counter);
    return 1;
}