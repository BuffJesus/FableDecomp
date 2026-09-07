#pragma optimize("s",on)
struct InsertNode_00655af3{InsertNode_00655af3* next;InsertNode_00655af3* prev;unsigned long value;};
InsertNode_00655af3* __stdcall AllocateInsertNode_00655af3(unsigned long value);
InsertNode_00655af3** __stdcall std_list_insert_node_before(InsertNode_00655af3** output,InsertNode_00655af3* before,unsigned long value){InsertNode_00655af3* inserted=AllocateInsertNode_00655af3(value);InsertNode_00655af3* previous=before->prev;inserted->next=before;inserted->prev=previous;previous->next=inserted;before->prev=inserted;*output=inserted;return output;}