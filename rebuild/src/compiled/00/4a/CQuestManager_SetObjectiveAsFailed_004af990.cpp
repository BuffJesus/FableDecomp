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