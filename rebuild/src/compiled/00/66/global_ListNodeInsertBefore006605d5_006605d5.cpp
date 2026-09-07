#pragma optimize("s",on)
struct InsertNode_006605d5{InsertNode_006605d5* next;InsertNode_006605d5* prev;unsigned long value;};
InsertNode_006605d5* __stdcall AllocateInsertNode_006605d5(unsigned long value);
InsertNode_006605d5** __stdcall ListNode_InsertBefore_006605d5(InsertNode_006605d5** output,InsertNode_006605d5* before,unsigned long value){InsertNode_006605d5* inserted=AllocateInsertNode_006605d5(value);InsertNode_006605d5* previous=before->prev;inserted->next=before;inserted->prev=previous;previous->next=inserted;before->prev=inserted;*output=inserted;return output;}