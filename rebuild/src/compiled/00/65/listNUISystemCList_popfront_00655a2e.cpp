struct Node { Node* next; };
struct MyList;
struct ListIt {
    Node* _Ptr;
    ListIt() {}
    ListIt(Node* p) : _Ptr(p) {}
    ListIt(const ListIt& o) : _Ptr(o._Ptr) {}
};
struct MyList {
    Node* head;
    ListIt erase(ListIt _Where);
};
void __fastcall pop_front(MyList* self)
{
    self->erase(ListIt(self->head->next));
}