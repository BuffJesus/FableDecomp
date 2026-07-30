// CInputTypeXboxPadRightStickEvent::GetType
// Retail 0x004456b0: mov eax, 4 ; ret
// __fastcall: this in ecx (unused). Returns constant type id 4.
int __fastcall GetType(void* ecx)
{
    (void)ecx;
    return 4;
}