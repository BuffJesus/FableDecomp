#include <stdio.h>

struct ListNode { ListNode* next; };
struct CTCKraken {
    char pad[0x44];
    ListNode* list;
    int a, b, c;
};

static int calls;
void __fastcall Kraken_sub_7fc210(CTCKraken* s){ s->a = 1; calls |= 1; }
void __fastcall Kraken_sub_7fb610(CTCKraken* s){ s->b = 1; calls |= 2; }
void __fastcall Kraken_sub_7fb150(CTCKraken* s){ s->c = 1; calls |= 4; }

void __fastcall InitTentaclesAndHead(CTCKraken* self)
{
    ListNode* n = self->list;
    if (n->next == n) Kraken_sub_7fc210(self);
    Kraken_sub_7fb610(self);
    Kraken_sub_7fb150(self);
}

int main(){
    // empty list: head->next == head  -> firstCall runs
    CTCKraken k1; k1.a=k1.b=k1.c=0;
    ListNode selfnode; ListNode* pself = &selfnode; selfnode.next = pself; k1.list = pself;
    calls=0; InitTentaclesAndHead(&k1);
    if (calls != 7) { printf("FAIL selfref %d\n", calls); return 1; }

    // non-empty: head->next != head -> firstCall skipped
    CTCKraken k2; k2.a=k2.b=k2.c=0;
    ListNode hd, tail; ListNode* phd = &hd; ListNode* ptail = &tail; hd.next = ptail; k2.list = phd;
    calls=0; InitTentaclesAndHead(&k2);
    if (calls != 6) { printf("FAIL nonempty %d\n", calls); return 1; }

    printf("KRAKEN_INIT_OK\n");
    return 0;
}