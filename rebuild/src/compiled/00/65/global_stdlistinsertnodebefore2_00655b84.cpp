#pragma optimize("s",on)
struct InsertNode_00655b84{InsertNode_00655b84* next;InsertNode_00655b84* prev;unsigned long value;};
InsertNode_00655b84* __stdcall AllocateInsertNode_00655b84(unsigned long value);
InsertNode_00655b84** __stdcall std_list_insert_node_before_2(InsertNode_00655b84** output,InsertNode_00655b84* before,unsigned long value){InsertNode_00655b84* inserted=AllocateInsertNode_00655b84(value);InsertNode_00655b84* previous=before->prev;inserted->next=before;inserted->prev=previous;previous->next=inserted;before->prev=inserted;*output=inserted;return output;}