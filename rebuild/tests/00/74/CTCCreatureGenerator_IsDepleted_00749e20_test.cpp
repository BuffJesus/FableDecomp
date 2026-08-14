#include <cstdio>

struct ListNode { ListNode* next; };

struct Gen {
    char pad00[0x18];
    ListNode* list;
    char pad1c[0x40 - 0x1c];
    int f40;
    char pad44[0x54 - 0x44];
    int f54;
    char pad58[0x84 - 0x58];
    int f84;
};

static bool IsDepleted(Gen* self)
{
    ListNode* head = self->list;
    if (head->next == head) {          // list empty
        if (self->f84 >= self->f40) return true;
        if (self->f54 == 0) return true;
    }
    return false;
}

int main()
{
    ListNode selfNode; selfNode.next = &selfNode; // empty list (head->next == head)
    ListNode other;    other.next = &other;
    ListNode n;        n.next = &other;           // non-empty (head->next != head)

    Gen g;
    // Case A: non-empty -> NEVER depleted regardless of counts
    g.list = &n; g.f84 = 999; g.f40 = 100; g.f54 = 0;
    bool a = IsDepleted(&g);

    // Case B: empty, count below cap, f54!=0 -> NOT depleted
    g.list = &selfNode; g.f84 = 5; g.f40 = 100; g.f54 = 1;
    bool b = IsDepleted(&g);

    // Case C: empty, count >= cap -> depleted
    g.list = &selfNode; g.f84 = 100; g.f40 = 100; g.f54 = 1;
    bool c = IsDepleted(&g);

    // Case D: empty, below cap, f54==0 -> depleted
    g.list = &selfNode; g.f84 = 5; g.f40 = 100; g.f54 = 0;
    bool d = IsDepleted(&g);

    if (!a && !b && c && d) {
        printf("BEHAVIOUR_OK\n");
        return 0;
    }
    printf("BEHAVIOUR_BAD a=%d b=%d c=%d d=%d\n", a, b, c, d);
    return 1;
}