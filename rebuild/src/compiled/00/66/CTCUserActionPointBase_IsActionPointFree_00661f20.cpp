// Retail 0x00661F20. The action-point user list is circular: its sentinel
// points to itself when empty. A distinct first node means the point has a free/active entry.
struct CActionPointUserNode { CActionPointUserNode* Next; };
struct CTCUserActionPointBase {
    char m_leading[0x108];
    CActionPointUserNode* m_pUserListSentinel;
    bool IsActionPointFree() const;
};
bool CTCUserActionPointBase::IsActionPointFree() const {
    return m_pUserListSentinel->Next != m_pUserListSentinel;
}