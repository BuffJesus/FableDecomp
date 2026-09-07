#pragma optimize("s",on)
// CRegionDialog_Methods::GetTreeHandleFromMapIndex @ 005b200c
// Allocates a 0x1c-byte handle object, initializes its embedded
// CGuiControlTree sub-object (at +0x10) from the map index, returns it.

#include "engine/CRegionDialog.h"  // retyped onto the PDB layout; byte parity re-verified
struct CGuiControlTree;

// callee at 0x5b1b20: __fastcall(ecx=CGuiControlTree* self, edx=long index)
void __fastcall SubInit_5b1b20(CGuiControlTree* self, long index);

struct CGuiControlTree
{
    char pad[0xC];
};

// The 0x1c (28)-byte handle object.
struct TreeHandle
{
    char          head[0x10];   // 0x00..0x0F
    CGuiControlTree tree;        // 0x10..0x1B  (12 bytes)
};

struct CRegionDialog_Methods : CRegionDialog {
    TreeHandle* GetTreeHandleFromMapIndex(long mapIndex);
};

TreeHandle* CRegionDialog_Methods::GetTreeHandleFromMapIndex(long mapIndex)
{
    TreeHandle* h = (TreeHandle*)::operator new(sizeof(TreeHandle));
    SubInit_5b1b20(&h->tree, mapIndex);
    return h;
}