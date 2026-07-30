// CInputTypeMouseWheelMovementEvent::GetType
// Retail 0x00445730: mov eax,6 ; ret
// __fastcall (this in ecx) returning a constant type id.
int __fastcall GetType(void *ecx)
{
    (void)ecx;
    return 6;
}