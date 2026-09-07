struct CLandscapeBackgroundTreeNode {};
struct CLandscapeBackgroundSubNodeSplitBranch {
    char pad[0x14];
    CLandscapeBackgroundTreeNode* parent;
    CLandscapeBackgroundTreeNode* relocatedParent;
    void OnPostParentRelocation(CLandscapeBackgroundTreeNode* node);
};
void CLandscapeBackgroundSubNodeSplitBranch::OnPostParentRelocation(CLandscapeBackgroundTreeNode* node) {
    relocatedParent = node;
    parent = node;
}