#pragma optimize("s",on)
struct InsertNode_0053ca7b{InsertNode_0053ca7b* next;InsertNode_0053ca7b* prev;unsigned long value;};
InsertNode_0053ca7b* __stdcall AllocateInsertNode_0053ca7b(unsigned long value);
InsertNode_0053ca7b** __stdcall std_list_node_insert(InsertNode_0053ca7b** output,InsertNode_0053ca7b* before,unsigned long value){InsertNode_0053ca7b* inserted=AllocateInsertNode_0053ca7b(value);InsertNode_0053ca7b* previous=before->prev;inserted->next=before;inserted->prev=previous;previous->next=inserted;before->prev=inserted;*output=inserted;return output;}