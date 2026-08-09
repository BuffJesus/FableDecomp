struct CNavSwitchableLeafNode {
  char pad0[0xf];
  unsigned char f_0f;
  char pad1[0x18-0x10];
  unsigned char f_18;
};

bool __fastcall CNavSwitchableLeafNode_HasNeighbours(CNavSwitchableLeafNode* self)
{
  if (!self->f_0f)
    return self->f_18 != 0;
  return false;
}