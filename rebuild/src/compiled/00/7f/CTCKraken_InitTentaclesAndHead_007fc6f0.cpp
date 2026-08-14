// Byte-exact reconstruction of CTCKraken::InitTentaclesAndHead @ 0x007fc6f0

struct ListNode {
    ListNode* next;
};

struct CTCKraken {
    char pad[0x44];
    ListNode* list; // +0x44
};

// callees (thiscall members modeled as __fastcall free fns)
void __fastcall Kraken_sub_7fc210(CTCKraken* self);
void __fastcall Kraken_sub_7fb610(CTCKraken* self);
void __fastcall Kraken_sub_7fb150(CTCKraken* self);

void __fastcall InitTentaclesAndHead(CTCKraken* self)
{
    ListNode* n = self->list;
    if (n->next == n) {
        Kraken_sub_7fc210(self);
    }
    Kraken_sub_7fb610(self);
    Kraken_sub_7fb150(self);
}