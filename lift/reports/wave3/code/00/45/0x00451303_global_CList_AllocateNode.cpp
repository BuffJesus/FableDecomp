void** __fastcall CList_AllocateNode(void** outNode)
{
    *outNode = nullptr;
    *outNode = std::malloc(0x14);
    return outNode;
}