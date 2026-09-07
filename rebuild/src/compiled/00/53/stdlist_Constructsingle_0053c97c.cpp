#pragma optimize("s",on)
struct InsertNode_0053c97c{InsertNode_0053c97c* next;InsertNode_0053c97c* prev;unsigned long value;};
InsertNode_0053c97c* __stdcall AllocateInsertNode_0053c97c(unsigned long value);
InsertNode_0053c97c** __stdcall _Construct_single(InsertNode_0053c97c** output,InsertNode_0053c97c* before,unsigned long value){InsertNode_0053c97c* inserted=AllocateInsertNode_0053c97c(value);InsertNode_0053c97c* previous=before->prev;inserted->next=before;inserted->prev=previous;previous->next=inserted;before->prev=inserted;*output=inserted;return output;}