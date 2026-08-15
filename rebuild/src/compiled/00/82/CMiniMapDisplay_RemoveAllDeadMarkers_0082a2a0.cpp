// CMiniMapDisplay::RemoveAllDeadMarkers  0x0082a2a0  __fastcall(void)
// Circular doubly-linked list of marker nodes: node+0=next, node+4=prev,
// node+8 = embedded marker object.

struct Marker;               // embedded at node+8
struct Node { Node* next; Node* prev; };  // followed by Marker at +8

struct QueryResult { char pad[0x91]; unsigned char flags; };

// 0x00a01b50 : Marker::GetSomething(this=ecx) -> QueryResult*
extern QueryResult* __fastcall Marker_Query(Marker* self);
// 0x00a01c10 : Marker::~/cleanup(this=ecx)
extern void __fastcall Marker_Destroy(Marker* self);
// 0x00bfea14 : free(void*)
extern "C" void __cdecl fable_free(void* p);

struct CMiniMapDisplay {
    char pad[0x20];
    Node* head;   // +0x20 : sentinel node pointer
};

void __fastcall CMiniMapDisplay_RemoveAllDeadMarkers(CMiniMapDisplay* self)
{
    Node* head = self->head;
    Node* n = head->next;
    while (n != self->head)
    {
        Marker* m = (Marker*)((char*)n + 8);
        QueryResult* q = Marker_Query(m);
        if (q != 0 && (q->flags & 1) == 0)
        {
            n = n->next;
        }
        else
        {
            Node* nx = n->next;
            Node* pv = n->prev;
            pv->next = nx;
            nx->prev = pv;
            Marker_Destroy(m);
            fable_free(n);
            n = nx;
        }
    }
}