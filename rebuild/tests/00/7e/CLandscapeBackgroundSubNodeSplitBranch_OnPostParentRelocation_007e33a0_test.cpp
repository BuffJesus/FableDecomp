#include <stdio.h>
struct CLandscapeBackgroundTreeNode {};
struct CLandscapeBackgroundSubNodeSplitBranch {
    char pad[0x14];
    CLandscapeBackgroundTreeNode* parent;
    CLandscapeBackgroundTreeNode* relocatedParent;
    void OnPostParentRelocation(CLandscapeBackgroundTreeNode* node);
};
void CLandscapeBackgroundSubNodeSplitBranch::OnPostParentRelocation(CLandscapeBackgroundTreeNode* node) {
    relocatedParent = node; parent = node;
}
int main() {
    CLandscapeBackgroundTreeNode node;
    CLandscapeBackgroundSubNodeSplitBranch branch;
    branch.parent = branch.relocatedParent = 0;
    branch.OnPostParentRelocation(&node);
    if (branch.parent != &node || branch.relocatedParent != &node) return 1;
    printf("POST_PARENT_RELOCATION PASS\n");
    return 0;
}