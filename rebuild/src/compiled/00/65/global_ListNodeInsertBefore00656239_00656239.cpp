#pragma optimize("s",on)
struct InsertNode_00656239{InsertNode_00656239* next;InsertNode_00656239* prev;unsigned long value;};
InsertNode_00656239* __stdcall AllocateInsertNode_00656239(unsigned long value);
InsertNode_00656239** __stdcall ListNode_InsertBefore_00656239(InsertNode_00656239** output,InsertNode_00656239* before,unsigned long value){InsertNode_00656239* inserted=AllocateInsertNode_00656239(value);InsertNode_00656239* previous=before->prev;inserted->next=before;inserted->prev=previous;previous->next=inserted;before->prev=inserted;*output=inserted;return output;}