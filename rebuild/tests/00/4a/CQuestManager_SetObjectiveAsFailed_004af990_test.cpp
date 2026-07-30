#include <stdio.h>

struct QMNode
{
    QMNode*       next;    // 0x00
    QMNode*       prev;    // 0x04
    unsigned long pad08;   // 0x08
    unsigned long key;     // 0x0c
    unsigned long status;  // 0x10
};

class CQuestManager
{
    unsigned char m_pad[0x74];
    QMNode*       m_sentinel;  // 0x74
public:
    void SetObjectiveAsFailed(unsigned long objective);
    QMNode** sentinelSlot() { return &m_sentinel; }
};

void CQuestManager::SetObjectiveAsFailed(unsigned long objective)
{
    QMNode* sentinel = m_sentinel;
    for (QMNode* node = sentinel->next; node != sentinel; node = node->next)
    {
        if (node->key == objective)
        {
            node->status = 2;
            return;
        }
    }
}

// thiscall shim: this in ecx, one stack arg, ret 4
static void call_set(CQuestManager* self, unsigned long objective)
{
    void* fn = 0;
    {
        void (CQuestManager::*mp)(unsigned long) = &CQuestManager::SetObjectiveAsFailed;
        __asm { mov eax, mp }
        __asm { mov fn, eax }
    }
    __asm {
        push objective
        mov  ecx, self
        call fn
    }
}

int main()
{
    // Build circular list with sentinel + 3 real nodes.
    QMNode sentinel, n1, n2, n3;
    sentinel.next = &n1;
    n1.next = &n2; n2.next = &n3; n3.next = &sentinel;
    n1.key = 10; n2.key = 20; n3.key = 30;
    n1.status = 0; n2.status = 0; n3.status = 0;

    CQuestManager qm;
    *qm.sentinelSlot() = &sentinel;

    // Match in the middle: node with key 20 -> status becomes 2.
    call_set(&qm, 20);
    if (n1.status != 0 || n2.status != 2 || n3.status != 0)
    {
        printf("FAIL match: %lu %lu %lu\n", n1.status, n2.status, n3.status);
        return 1;
    }

    // Reset, test no-match: nothing changes.
    n2.status = 0;
    call_set(&qm, 999);
    if (n1.status != 0 || n2.status != 0 || n3.status != 0)
    {
        printf("FAIL nomatch\n");
        return 1;
    }

    // Match last node.
    call_set(&qm, 30);
    if (n3.status != 2)
    {
        printf("FAIL last\n");
        return 1;
    }

    printf("OK_0x004af990\n");
    return 0;
}