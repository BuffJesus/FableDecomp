#include <cstdio>

struct ListNode { ListNode* next; };
struct CTCKraken {
    char pad[0x44];
    ListNode* list;
    int cleared, setupT, setupH;
};

void __fastcall CTCKraken_ClearTentacles(CTCKraken* self){ self->cleared++; }
void __fastcall CTCKraken_SetupTentacles(CTCKraken* self){ self->setupT++; }
void __fastcall CTCKraken_SetupHead(CTCKraken* self){ self->setupH++; }

void __fastcall InitTentaclesAndHead(CTCKraken* self)
{
    ListNode* head = self->list;
    if (head->next != (ListNode*)head)
        CTCKraken_ClearTentacles(self);
    CTCKraken_SetupTentacles(self);
    CTCKraken_SetupHead(self);
}

int main()
{
    // empty list: head->next == head -> no clear
    ListNode selfhead; selfhead.next = &selfhead;
    CTCKraken a; a.list = &selfhead; a.cleared=a.setupT=a.setupH=0;
    InitTentaclesAndHead(&a);

    // non-empty: head->next != head -> clear runs
    ListNode n2; ListNode h; h.next = &n2;
    CTCKraken b; b.list = &h; b.cleared=b.setupT=b.setupH=0;
    InitTentaclesAndHead(&b);

    if (a.cleared==0 && a.setupT==1 && a.setupH==1 &&
        b.cleared==1 && b.setupT==1 && b.setupH==1)
        printf("KRAKEN_INIT_OK\n");
    else
        printf("FAIL\n");
    return 0;
}