// Byte-exact reconstruction of CTCCreatureGenerator::IsDepleted @ 0x00749e20

struct ListNode {
    ListNode* next; // +0x00
};

struct CTCCreatureGenerator {
    char pad00[0x18];
    ListNode* list;   // +0x18  (intrusive list head; empty when head->next == head)
    char pad1c[0x40 - 0x1c];
    int   f40;        // +0x40
    char pad44[0x54 - 0x44];
    int   f54;        // +0x54
    char pad58[0x84 - 0x58];
    int   f84;        // +0x84
};

bool __fastcall IsDepleted(CTCCreatureGenerator* self)
{
    ListNode* head = self->list;
    if (head->next == head)          // list empty
    {
        if (self->f84 >= self->f40)  // count reached cap -> depleted
            return true;
        if (self->f54 == 0)          // no active generator -> depleted
            return true;
    }
    return false;
}