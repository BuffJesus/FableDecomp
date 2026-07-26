int __fastcall CNavNavigableLeafNode_GetRegion(const unsigned char* self)
{
    return *reinterpret_cast<const int*>(self + 16);
}