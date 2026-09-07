#pragma optimize("s",on)
struct InsertNode_005d8626{InsertNode_005d8626* next;InsertNode_005d8626* prev;unsigned long value;};
InsertNode_005d8626* __stdcall AllocateInsertNode_005d8626(unsigned long value);
InsertNode_005d8626** __stdcall ListNode_InsertBefore_005d8626(InsertNode_005d8626** output,InsertNode_005d8626* before,unsigned long value){InsertNode_005d8626* inserted=AllocateInsertNode_005d8626(value);InsertNode_005d8626* previous=before->prev;inserted->next=before;inserted->prev=previous;previous->next=inserted;before->prev=inserted;*output=inserted;return output;}