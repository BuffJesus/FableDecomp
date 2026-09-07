#pragma optimize("s",on)
struct InsertNode_005931b5{InsertNode_005931b5* next;InsertNode_005931b5* prev;unsigned long value;};
InsertNode_005931b5* __stdcall AllocateInsertNode_005931b5(unsigned long value);
InsertNode_005931b5** __stdcall DoublyLinkedList_Insert(InsertNode_005931b5** output,InsertNode_005931b5* before,unsigned long value){InsertNode_005931b5* inserted=AllocateInsertNode_005931b5(value);InsertNode_005931b5* previous=before->prev;inserted->next=before;inserted->prev=previous;previous->next=inserted;before->prev=inserted;*output=inserted;return output;}