#pragma optimize("s",on)
struct InsertNode_0065fd39{InsertNode_0065fd39* next;InsertNode_0065fd39* prev;unsigned long value;};
InsertNode_0065fd39* __stdcall AllocateInsertNode_0065fd39(unsigned long value);
InsertNode_0065fd39** __stdcall ListNode_InsertBefore_0065fd39(InsertNode_0065fd39** output,InsertNode_0065fd39* before,unsigned long value){InsertNode_0065fd39* inserted=AllocateInsertNode_0065fd39(value);InsertNode_0065fd39* previous=before->prev;inserted->next=before;inserted->prev=previous;previous->next=inserted;before->prev=inserted;*output=inserted;return output;}