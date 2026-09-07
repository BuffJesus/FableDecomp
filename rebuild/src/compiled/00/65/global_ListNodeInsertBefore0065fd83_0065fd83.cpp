#pragma optimize("s",on)
struct InsertNode_0065fd83{InsertNode_0065fd83* next;InsertNode_0065fd83* prev;unsigned long value;};
InsertNode_0065fd83* __stdcall AllocateInsertNode_0065fd83(unsigned long value);
InsertNode_0065fd83** __stdcall ListNode_InsertBefore_0065fd83(InsertNode_0065fd83** output,InsertNode_0065fd83* before,unsigned long value){InsertNode_0065fd83* inserted=AllocateInsertNode_0065fd83(value);InsertNode_0065fd83* previous=before->prev;inserted->next=before;inserted->prev=previous;previous->next=inserted;before->prev=inserted;*output=inserted;return output;}