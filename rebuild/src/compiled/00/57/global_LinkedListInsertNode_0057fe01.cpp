#pragma optimize("s",on)
struct InsertNode_0057fe01{InsertNode_0057fe01* next;InsertNode_0057fe01* prev;unsigned long value;};
InsertNode_0057fe01* __stdcall AllocateInsertNode_0057fe01(unsigned long value);
InsertNode_0057fe01** __stdcall LinkedList_InsertNode(InsertNode_0057fe01** output,InsertNode_0057fe01* before,unsigned long value){InsertNode_0057fe01* inserted=AllocateInsertNode_0057fe01(value);InsertNode_0057fe01* previous=before->prev;inserted->next=before;inserted->prev=previous;previous->next=inserted;before->prev=inserted;*output=inserted;return output;}