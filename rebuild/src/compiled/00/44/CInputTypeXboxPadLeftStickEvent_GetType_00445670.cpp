// CInputTypeXboxPadLeftStickEvent::GetType @ 0x00445670
// Retail: b8 03 00 00 00 c3  => mov eax,3 ; ret
// Virtual event-type accessor: returns the constant type id 3.
// __fastcall (this in ecx) but 'this' is unused; no stack args => ret 0.
int __fastcall GetType(void *ecx)
{
    (void)ecx;
    return 3;
}