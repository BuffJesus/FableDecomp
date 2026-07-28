void** __fastcall RBTreeIterator_MoveToMax_D(void** iterator)
{
    *iterator = RBTree__GetMaxNode(*iterator);
    return iterator;
}