#pragma optimize("s",on)
struct InsertNode_00655bc4{InsertNode_00655bc4* next;InsertNode_00655bc4* prev;unsigned long value;};
InsertNode_00655bc4* __stdcall AllocateInsertNode_00655bc4(unsigned long value);
InsertNode_00655bc4** __stdcall std_list_insert_node_before_3(InsertNode_00655bc4** output,InsertNode_00655bc4* before,unsigned long value){InsertNode_00655bc4* inserted=AllocateInsertNode_00655bc4(value);InsertNode_00655bc4* previous=before->prev;inserted->next=before;inserted->prev=previous;previous->next=inserted;before->prev=inserted;*output=inserted;return output;}