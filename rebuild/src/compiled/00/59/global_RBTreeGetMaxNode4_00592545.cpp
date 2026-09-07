#pragma optimize("s",on)
extern void* __cdecl RBTree__GetMaxNode(void* node);
void** __fastcall RBTree_GetMaxNode_4(void** iterator){*iterator=RBTree__GetMaxNode(*iterator);return iterator;}