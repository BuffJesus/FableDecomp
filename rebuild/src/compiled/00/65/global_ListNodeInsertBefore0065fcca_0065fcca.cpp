#pragma optimize("s",on)
struct InsertNode_0065fcca{InsertNode_0065fcca* next;InsertNode_0065fcca* prev;unsigned long value;};
InsertNode_0065fcca* __stdcall AllocateInsertNode_0065fcca(unsigned long value);
InsertNode_0065fcca** __stdcall ListNode_InsertBefore_0065fcca(InsertNode_0065fcca** output,InsertNode_0065fcca* before,unsigned long value){InsertNode_0065fcca* inserted=AllocateInsertNode_0065fcca(value);InsertNode_0065fcca* previous=before->prev;inserted->next=before;inserted->prev=previous;previous->next=inserted;before->prev=inserted;*output=inserted;return output;}