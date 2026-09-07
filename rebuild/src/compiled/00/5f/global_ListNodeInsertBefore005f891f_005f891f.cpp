#pragma optimize("s",on)
struct InsertNode_005f891f{InsertNode_005f891f* next;InsertNode_005f891f* prev;unsigned long value;};
InsertNode_005f891f* __stdcall AllocateInsertNode_005f891f(unsigned long value);
InsertNode_005f891f** __stdcall ListNode_InsertBefore_005f891f(InsertNode_005f891f** output,InsertNode_005f891f* before,unsigned long value){InsertNode_005f891f* inserted=AllocateInsertNode_005f891f(value);InsertNode_005f891f* previous=before->prev;inserted->next=before;inserted->prev=previous;previous->next=inserted;before->prev=inserted;*output=inserted;return output;}