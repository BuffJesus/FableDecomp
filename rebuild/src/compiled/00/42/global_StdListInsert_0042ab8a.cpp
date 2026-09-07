#pragma optimize("s",on)
struct StdListInsertNode
{
    StdListInsertNode* next;
    StdListInsertNode* prev;
};

StdListInsertNode* __stdcall Std_List_AllocateNode(unsigned long value);

StdListInsertNode** __stdcall Std_List_Insert(
    StdListInsertNode** out_inserted,
    StdListInsertNode* node,
    unsigned long value)
{
    StdListInsertNode* inserted = Std_List_AllocateNode(value);
    StdListInsertNode* previous = node->prev;
    inserted->next = node;
    inserted->prev = previous;
    previous->next = inserted;
    node->prev = inserted;
    *out_inserted = inserted;
    return out_inserted;
}