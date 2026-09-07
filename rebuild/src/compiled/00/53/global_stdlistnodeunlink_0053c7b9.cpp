#pragma optimize("s",on)
#include <stdlib.h>
struct LinkedListNodeOverlay{LinkedListNodeOverlay* prev;LinkedListNodeOverlay* next;};
void __stdcall std_list_node_unlink(LinkedListNodeOverlay** cursor,LinkedListNodeOverlay* node){LinkedListNodeOverlay* next=node->next;LinkedListNodeOverlay* prev=node->prev;next->prev=prev;prev->next=next;free(node);*cursor=prev;}