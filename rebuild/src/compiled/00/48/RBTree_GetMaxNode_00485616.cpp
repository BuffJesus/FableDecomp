#pragma optimize("s",on)
extern void* __cdecl RBTree__GetMaxNode(void* node);
void** __fastcall GetMaxNode(void** iterator){*iterator=RBTree__GetMaxNode(*iterator);return iterator;}