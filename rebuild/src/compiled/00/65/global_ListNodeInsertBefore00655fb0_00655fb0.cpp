#pragma optimize("s",on)
struct InsertNode_00655fb0{InsertNode_00655fb0* next;InsertNode_00655fb0* prev;unsigned long value;};
InsertNode_00655fb0* __stdcall AllocateInsertNode_00655fb0(unsigned long value);
InsertNode_00655fb0** __stdcall ListNode_InsertBefore_00655fb0(InsertNode_00655fb0** output,InsertNode_00655fb0* before,unsigned long value){InsertNode_00655fb0* inserted=AllocateInsertNode_00655fb0(value);InsertNode_00655fb0* previous=before->prev;inserted->next=before;inserted->prev=previous;previous->next=inserted;before->prev=inserted;*output=inserted;return output;}