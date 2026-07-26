
struct Thing { char pad[0x91]; unsigned char flags; };
struct Node { Node* next; void* pad4; Thing* thing; };
struct SubFilter { bool Test(Thing* t); };
struct Filter { SubFilter* f0; SubFilter* f1; };
struct List { Node* sentinel; };

extern List* __stdcall GetThingList(int kind);

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