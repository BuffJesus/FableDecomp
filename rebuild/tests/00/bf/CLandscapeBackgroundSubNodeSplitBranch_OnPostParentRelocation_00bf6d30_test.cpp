#include "rebuild_abi.h"
#include <cstdio>
struct CLandscapeBackgroundTreeNode;
struct CLandscapeBackgroundSubNodeSplitBranch {
    char pad00[0xa0]; CLandscapeBackgroundTreeNode *f_a0; char padA4[0x188-0xa4]; CLandscapeBackgroundTreeNode *f_188;
    void OnPostParentRelocation(CLandscapeBackgroundTreeNode *parent);
};
void CLandscapeBackgroundSubNodeSplitBranch::OnPostParentRelocation(CLandscapeBackgroundTreeNode *parent){ f_a0=parent; f_188=parent; }
int main(){
    CLandscapeBackgroundSubNodeSplitBranch n; n.f_a0=0; n.f_188=0;
    CLandscapeBackgroundTreeNode *p=(CLandscapeBackgroundTreeNode*)0x1234;
    n.OnPostParentRelocation(p);
    if(n.f_a0!=p||n.f_188!=p){std::printf("BAD\n");return 1;}
    std::printf("ONPOST_OK\n"); return 0;
}