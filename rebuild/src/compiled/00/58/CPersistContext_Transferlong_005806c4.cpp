#pragma optimize("s",on)
// CPersistContext::Transfer<long>(char* name, long* value)
// Retail 0x005806c4. Genuine __fastcall member (this in ecx, unused/unspilled),
// 2 stack args, ret 8. Zeroes a local temp, forwards (name, value, &temp) to
// cdecl helper 0x00580371.

extern "C" void __stdcall PersistHelper_580371(char* name, long* value, long* out);

struct CPersistContext
{
    void Transfer_long_(char* name, long* value);
};

void CPersistContext::Transfer_long_(char* name, long* value)
{
    long temp = 0;
    PersistHelper_580371(name, value, &temp);
}