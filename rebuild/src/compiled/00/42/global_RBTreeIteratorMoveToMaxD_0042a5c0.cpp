#pragma optimize("s",on)
extern void* __cdecl RBTree__GetMaxNode(void* node);
void** __fastcall RBTreeIterator_MoveToMax_D(void** iterator){*iterator=RBTree__GetMaxNode(*iterator);return iterator;}