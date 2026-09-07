#pragma optimize("s",on)
struct InsertNode_0065fd5e{InsertNode_0065fd5e* next;InsertNode_0065fd5e* prev;unsigned long value;};
InsertNode_0065fd5e* __stdcall AllocateInsertNode_0065fd5e(unsigned long value);
InsertNode_0065fd5e** __stdcall ListNode_InsertBefore_0065fd5e(InsertNode_0065fd5e** output,InsertNode_0065fd5e* before,unsigned long value){InsertNode_0065fd5e* inserted=AllocateInsertNode_0065fd5e(value);InsertNode_0065fd5e* previous=before->prev;inserted->next=before;inserted->prev=previous;previous->next=inserted;before->prev=inserted;*output=inserted;return output;}