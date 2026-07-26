
#include <cstdio>
struct Thing { char pad[0x91]; unsigned char flags; };
struct Node { Node* next; void* pad4; Thing* thing; };
struct SubFilter { int id; bool Test(Thing* t); };
struct Filter { SubFilter* f0; SubFilter* f1; };
struct List { Node* sentinel; };

List g_list;
Node g_sentinel;
static Thing t0, t1, t2;
static SubFilter sfa, sfb;

bool SubFilter::Test(Thing* t){ (void)t; return true; }
List* __stdcall GetThingList(int kind){ (void)kind; return &g_list; }

bool __fastcall CThingSearchTools_GetPFirstLeafThing(void* self, int edx, Filter* filter, Thing** out)
{
    List* lst = GetThingList(0xf);
    Node* n = lst->sentinel->next;
    if (n != lst->sentinel) {
        do {
            Thing* t = n->thing;
            unsigned char al = t->flags;
            unsigned char cl = ~al;
            if ((al & 4) && (al & 8) && (cl & 1) && !(al & 0x20)) {
                if (filter->f0->Test(t)) {
                    if (filter->f1->Test(t)) {
                        *out = t;
                        return true;
                    }
                }
            }
            n = n->next;
        } while (n != lst->sentinel);
    }
    return false;
}

int main(){
    // build circular list: sentinel -> n0 -> n1 -> n2 -> sentinel
    static Node n0, n1, n2;
    g_list.sentinel = &g_sentinel;
    g_sentinel.next = &n0;
    n0.next = &n1; n1.next = &n2; n2.next = &g_sentinel;
    n0.thing = &t0; n1.thing = &t1; n2.thing = &t2;
    // t0 fails (bit0 set), t1 passes, t2 passes
    t0.flags = 0x04 | 0x08;             // bit0 clear? this sets 0x0C -> passes actually
    t0.flags = 0x0C | 0x01;             // bit0 set -> fail
    t1.flags = 0x0C;                    // passes
    t2.flags = 0x0C;
    Filter f; f.f0=&sfa; f.f1=&sfb;

    Thing* out = 0;
    bool r = CThingSearchTools_GetPFirstLeafThing(0,0,&f,&out);
    if (r && out == &t1) {
        std::printf("CThingSearchTools_004a9ef0_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL r=%d out=%p\n", (int)r, (void*)out);
    return 1;
}