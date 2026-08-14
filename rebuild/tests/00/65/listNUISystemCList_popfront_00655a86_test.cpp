#include <cstdio>

struct Node { Node* next; };
struct MyList;
struct ListIt {
    Node* _Ptr;
    ListIt() {}
    ListIt(Node* p) : _Ptr(p) {}
    ListIt(const ListIt& o) : _Ptr(o._Ptr) {}
};

static Node* g_erased_ptr = 0;
static MyList* g_self = 0;

struct MyList {
    Node* head;
    ListIt erase(ListIt _Where) { g_erased_ptr = _Where._Ptr; g_self = this; return _Where; }
};

void __fastcall pop_front(MyList* self)
{
    self->erase(ListIt(self->head->next));
}

int main()
{
    Node n0, n1;
    n0.next = &n1; n1.next = 0;
    MyList lst; lst.head = &n0;   // begin() == head->next == &n1
    pop_front(&lst);
    if (g_erased_ptr == &n1 && g_self == &lst) printf("PARITY_OK\n");
    else printf("FAIL\n");
    return 0;
}