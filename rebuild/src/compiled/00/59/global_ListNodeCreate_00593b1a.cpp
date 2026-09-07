#pragma optimize("s",on)
struct InsertNode_00593b1a{InsertNode_00593b1a* next;InsertNode_00593b1a* prev;unsigned long value;};
InsertNode_00593b1a* __stdcall AllocateInsertNode_00593b1a(unsigned long value);
InsertNode_00593b1a** __stdcall ListNode_Create(InsertNode_00593b1a** output,InsertNode_00593b1a* before,unsigned long value){InsertNode_00593b1a* inserted=AllocateInsertNode_00593b1a(value);InsertNode_00593b1a* previous=before->prev;inserted->next=before;inserted->prev=previous;previous->next=inserted;before->prev=inserted;*output=inserted;return output;}