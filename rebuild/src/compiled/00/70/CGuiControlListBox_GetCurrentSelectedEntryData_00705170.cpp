struct Entry { char pad[4]; unsigned long data; };
struct S { char pad[0x10]; Entry* cur; };
unsigned long __fastcall GetCurrentSelectedEntryData(S* self)
{
    Entry* e = self->cur;
    return e ? e->data : 0;
}