#include "engine/CLandscapeBackgroundSubNodeSplitBranch.h"

struct RelocatableLandscapeNode { unsigned char data[4]; };
void __fastcall HelperRelocate(RelocatableLandscapeNode* node);

struct CLandscapeBackgroundSubNodeSplitBranch_Methods : CLandscapeBackgroundSubNodeSplitBranch {
    virtual void OnPostRelocation();
};

void CLandscapeBackgroundSubNodeSplitBranch_Methods::OnPostRelocation() {
    CLandscapeBackgroundSubNodeSplitBranch* branch =
        reinterpret_cast<CLandscapeBackgroundSubNodeSplitBranch*>(this);
    HelperRelocate(reinterpret_cast<RelocatableLandscapeNode*>(branch->LeftBranch));
    HelperRelocate(reinterpret_cast<RelocatableLandscapeNode*>(branch->LeftBranch + 0xe8));
}
