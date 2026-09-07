#pragma optimize("s",on)
extern void* __cdecl RBTree__GetMaxNode(void* node);
void** __fastcall RBTree_Iterator_SetToMaxNode_005ba8be(void** iterator){*iterator=RBTree__GetMaxNode(*iterator);return iterator;}