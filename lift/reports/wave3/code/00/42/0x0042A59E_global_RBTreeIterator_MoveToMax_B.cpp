void** __fastcall RBTreeIterator_MoveToMax_B(void** iterator)
{
    *iterator = RBTree__GetMaxNode(*iterator);
    return iterator;
}