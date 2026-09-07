#pragma optimize("s",on)
struct InsertNode_00493c7d{InsertNode_00493c7d* next;InsertNode_00493c7d* prev;unsigned long value;};
InsertNode_00493c7d* __stdcall AllocateInsertNode_00493c7d(unsigned long value);
InsertNode_00493c7d** __stdcall ListNode_Insert_AtPosition(InsertNode_00493c7d** output,InsertNode_00493c7d* before,unsigned long value){InsertNode_00493c7d* inserted=AllocateInsertNode_00493c7d(value);InsertNode_00493c7d* previous=before->prev;inserted->next=before;inserted->prev=previous;previous->next=inserted;before->prev=inserted;*output=inserted;return output;}