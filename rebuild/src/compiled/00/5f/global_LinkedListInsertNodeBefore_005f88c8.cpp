#pragma optimize("s",on)
struct InsertNode_005f88c8{InsertNode_005f88c8* next;InsertNode_005f88c8* prev;unsigned long value;};
InsertNode_005f88c8* __stdcall AllocateInsertNode_005f88c8(unsigned long value);
InsertNode_005f88c8** __stdcall LinkedList_InsertNodeBefore(InsertNode_005f88c8** output,InsertNode_005f88c8* before,unsigned long value){InsertNode_005f88c8* inserted=AllocateInsertNode_005f88c8(value);InsertNode_005f88c8* previous=before->prev;inserted->next=before;inserted->prev=previous;previous->next=inserted;before->prev=inserted;*output=inserted;return output;}