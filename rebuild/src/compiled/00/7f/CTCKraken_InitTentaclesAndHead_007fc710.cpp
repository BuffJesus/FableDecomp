// CTCKraken::InitTentaclesAndHead @ 0x007fc710
struct ListNode { ListNode* next; };
struct CTCKraken {
    char pad[0x44];
    ListNode* list; // +0x44
};

void __fastcall CTCKraken_ClearTentacles(CTCKraken* self);   // 0x7fc210
void __fastcall CTCKraken_SetupTentacles(CTCKraken* self);   // 0x7fb610
void __fastcall CTCKraken_SetupHead(CTCKraken* self);        // 0x7fb150

void __fastcall InitTentaclesAndHead(CTCKraken* self)
{
    ListNode* head = self->list;
    if (head->next == (ListNode*)head)
        CTCKraken_ClearTentacles(self);
    CTCKraken_SetupTentacles(self);
    CTCKraken_SetupHead(self);
}