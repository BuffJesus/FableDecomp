#pragma optimize("s",on)
struct InsertNode_0057fd7d{InsertNode_0057fd7d* next;InsertNode_0057fd7d* prev;unsigned long value;};
InsertNode_0057fd7d* __stdcall AllocateInsertNode_0057fd7d(unsigned long value);
InsertNode_0057fd7d** __stdcall ListNode_InsertBefore_0057fd7d(InsertNode_0057fd7d** output,InsertNode_0057fd7d* before,unsigned long value){InsertNode_0057fd7d* inserted=AllocateInsertNode_0057fd7d(value);InsertNode_0057fd7d* previous=before->prev;inserted->next=before;inserted->prev=previous;previous->next=inserted;before->prev=inserted;*output=inserted;return output;}