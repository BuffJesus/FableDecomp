#pragma optimize("s",on)
struct InsertNode_00593ade{InsertNode_00593ade* next;InsertNode_00593ade* prev;unsigned long value;};
InsertNode_00593ade* __stdcall AllocateInsertNode_00593ade(unsigned long value);
InsertNode_00593ade** __stdcall ListNode_InsertBefore_00593ade(InsertNode_00593ade** output,InsertNode_00593ade* before,unsigned long value){InsertNode_00593ade* inserted=AllocateInsertNode_00593ade(value);InsertNode_00593ade* previous=before->prev;inserted->next=before;inserted->prev=previous;previous->next=inserted;before->prev=inserted;*output=inserted;return output;}