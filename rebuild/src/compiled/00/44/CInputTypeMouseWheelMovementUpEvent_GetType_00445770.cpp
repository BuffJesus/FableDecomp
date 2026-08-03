// CInputTypeMouseWheelMovementUpEvent::GetType
// retail 0x00445770: b8 07 00 00 00 c3  ->  mov eax,7 ; ret
// __fastcall virtual returning a constant type id (7); 'this' in ecx, unused.
int __fastcall GetType(void* ecx)
{
    (void)ecx;
    return 7;
}