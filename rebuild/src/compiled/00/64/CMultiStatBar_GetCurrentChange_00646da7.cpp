struct Node {
    Node* volatile next;   // +0x00
    long  pad;             // +0x04
    long  pad2;            // +0x08
    long  value;           // +0x0c
};

struct CMultiStatBar {
    char  pad0[0x28];
    long  mode;            // +0x28
    char  pad1[0x68 - 0x2c];
    Node* head;            // +0x68
};

long __fastcall GetCurrentChange(CMultiStatBar* self)
{
    if (self->mode != 1)
    {
        Node* h = self->head;
        if (h->next != h)
            return h->next->value;
    }
    return 0;
}