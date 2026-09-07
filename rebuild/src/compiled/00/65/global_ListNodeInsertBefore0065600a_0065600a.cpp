#pragma optimize("s",on)
struct InsertNode_0065600a{InsertNode_0065600a* next;InsertNode_0065600a* prev;unsigned long value;};
InsertNode_0065600a* __stdcall AllocateInsertNode_0065600a(unsigned long value);
InsertNode_0065600a** __stdcall ListNode_InsertBefore_0065600a(InsertNode_0065600a** output,InsertNode_0065600a* before,unsigned long value){InsertNode_0065600a* inserted=AllocateInsertNode_0065600a(value);InsertNode_0065600a* previous=before->prev;inserted->next=before;inserted->prev=previous;previous->next=inserted;before->prev=inserted;*output=inserted;return output;}