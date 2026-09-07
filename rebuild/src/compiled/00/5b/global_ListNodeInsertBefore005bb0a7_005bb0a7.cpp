#pragma optimize("s",on)
struct InsertNode_005bb0a7{InsertNode_005bb0a7* next;InsertNode_005bb0a7* prev;unsigned long value;};
InsertNode_005bb0a7* __stdcall AllocateInsertNode_005bb0a7(unsigned long value);
InsertNode_005bb0a7** __stdcall ListNode_InsertBefore_005bb0a7(InsertNode_005bb0a7** output,InsertNode_005bb0a7* before,unsigned long value){InsertNode_005bb0a7* inserted=AllocateInsertNode_005bb0a7(value);InsertNode_005bb0a7* previous=before->prev;inserted->next=before;inserted->prev=previous;previous->next=inserted;before->prev=inserted;*output=inserted;return output;}