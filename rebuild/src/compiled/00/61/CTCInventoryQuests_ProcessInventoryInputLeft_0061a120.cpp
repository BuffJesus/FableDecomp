// Byte-exact reconstruction of CTCInventoryQuests::ProcessInventoryInputLeft @ 0x0061a120
// __fastcall (self in ecx), void return.

struct CTCInventoryQuests {
    char pad[0x157];
    unsigned char m_flag;   // +0x157
};

// int-returning member helper @ 0x5bce2f (this in ecx)
int __fastcall Helper_5bce2f(CTCInventoryQuests* self);
// void-returning member helper @ 0x5bc638 (this in ecx)
void __fastcall Helper_5bc638(CTCInventoryQuests* self);

void __fastcall ProcessInventoryInputLeft(CTCInventoryQuests* self)
{
    if (self->m_flag != 0)
        return;
    if (Helper_5bce2f(self) <= 0)
        return;
    Helper_5bc638(self);
}