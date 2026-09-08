
#include "engine/CNavSwitchableLeafNode.h"  // retyped onto the PDB layout; byte parity re-verified
bool __fastcall CNavSwitchableLeafNode_HasNeighbours(CNavSwitchableLeafNode* self)
{
  if (!self->m_disabled)
    return self->f_18 != 0;
  return false;
}