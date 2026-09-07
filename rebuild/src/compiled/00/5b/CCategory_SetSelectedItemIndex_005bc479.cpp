#pragma optimize("s",on)
#include "engine/CCategory.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCategory_Methods : CCategory {
    void SetSelectedItemIndex(long index);
};

void CCategory_Methods::SetSelectedItemIndex(long index)
{
    this->GlobalDefIndex = index;
    this->SelectedItemIndex = index;
    if (index == -1)
        this->SelectedItemIndex = 0;
}