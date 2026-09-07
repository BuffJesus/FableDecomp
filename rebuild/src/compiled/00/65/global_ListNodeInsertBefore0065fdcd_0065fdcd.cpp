#pragma optimize("s",on)
struct InsertNode_0065fdcd{InsertNode_0065fdcd* next;InsertNode_0065fdcd* prev;unsigned long value;};
InsertNode_0065fdcd* __stdcall AllocateInsertNode_0065fdcd(unsigned long value);
InsertNode_0065fdcd** __stdcall ListNode_InsertBefore_0065fdcd(InsertNode_0065fdcd** output,InsertNode_0065fdcd* before,unsigned long value){InsertNode_0065fdcd* inserted=AllocateInsertNode_0065fdcd(value);InsertNode_0065fdcd* previous=before->prev;inserted->next=before;inserted->prev=previous;previous->next=inserted;before->prev=inserted;*output=inserted;return output;}