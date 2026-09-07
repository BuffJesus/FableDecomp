#pragma optimize("s",on)
struct InsertNode_00655f8b{InsertNode_00655f8b* next;InsertNode_00655f8b* prev;unsigned long value;};
InsertNode_00655f8b* __stdcall AllocateInsertNode_00655f8b(unsigned long value);
InsertNode_00655f8b** __stdcall ListNode_InsertBefore_00655f8b(InsertNode_00655f8b** output,InsertNode_00655f8b* before,unsigned long value){InsertNode_00655f8b* inserted=AllocateInsertNode_00655f8b(value);InsertNode_00655f8b* previous=before->prev;inserted->next=before;inserted->prev=previous;previous->next=inserted;before->prev=inserted;*output=inserted;return output;}