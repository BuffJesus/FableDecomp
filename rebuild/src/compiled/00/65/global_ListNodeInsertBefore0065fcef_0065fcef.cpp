#pragma optimize("s",on)
struct InsertNode_0065fcef{InsertNode_0065fcef* next;InsertNode_0065fcef* prev;unsigned long value;};
InsertNode_0065fcef* __stdcall AllocateInsertNode_0065fcef(unsigned long value);
InsertNode_0065fcef** __stdcall ListNode_InsertBefore_0065fcef(InsertNode_0065fcef** output,InsertNode_0065fcef* before,unsigned long value){InsertNode_0065fcef* inserted=AllocateInsertNode_0065fcef(value);InsertNode_0065fcef* previous=before->prev;inserted->next=before;inserted->prev=previous;previous->next=inserted;before->prev=inserted;*output=inserted;return output;}