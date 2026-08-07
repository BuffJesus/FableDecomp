#include "rebuild_abi.h"
// CLandscapeBackgroundSubNodeSplitBranch::OnPostParentRelocation @ 0x00bf6d30
// mov eax,[esp+4]; mov [ecx+0xa0],eax; mov [ecx+0x188],eax; ret 4
// Store the same parent pointer into two node fields.
struct CLandscapeBackgroundTreeNode;
struct CLandscapeBackgroundSubNodeSplitBranch {
    char pad00[0xa0];
    CLandscapeBackgroundTreeNode *f_a0;   // +0xa0
    char padA4[0x188 - 0xa4];
    CLandscapeBackgroundTreeNode *f_188;  // +0x188
    void OnPostParentRelocation(CLandscapeBackgroundTreeNode *parent);
};
void CLandscapeBackgroundSubNodeSplitBranch::OnPostParentRelocation(CLandscapeBackgroundTreeNode *parent)
{
    f_a0 = parent;
    f_188 = parent;
}