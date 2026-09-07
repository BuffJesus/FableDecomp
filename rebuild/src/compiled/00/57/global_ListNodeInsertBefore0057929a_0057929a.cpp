#pragma optimize("s",on)
struct InsertNode_0057929a{InsertNode_0057929a* next;InsertNode_0057929a* prev;unsigned long value;};
InsertNode_0057929a* __stdcall AllocateInsertNode_0057929a(unsigned long value);
InsertNode_0057929a** __stdcall ListNode_InsertBefore_0057929a(InsertNode_0057929a** output,InsertNode_0057929a* before,unsigned long value){InsertNode_0057929a* inserted=AllocateInsertNode_0057929a(value);InsertNode_0057929a* previous=before->prev;inserted->next=before;inserted->prev=previous;previous->next=inserted;before->prev=inserted;*output=inserted;return output;}