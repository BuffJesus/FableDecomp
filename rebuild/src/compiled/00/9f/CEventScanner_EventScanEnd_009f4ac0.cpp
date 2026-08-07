struct S { char pad[0x20]; int cnt; };
void __fastcall EventScanEnd(S* self, void* edx, unsigned char* p)
{
    (void)edx;
    *p = 0;
    self->cnt--;
}