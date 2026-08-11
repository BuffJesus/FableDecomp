#pragma optimize("s",on)
// CPersistContext::Transfer<long>(char* name, long* value) @ 0x005cc300
// 2-arg wrapper: builds a zeroed default and forwards to the 3-arg
// Transfer<long>(name, value, defaultValue) primitive at 0x5cc1e8.

struct CPersistContext
{
    // The 3-arg primitive (out-of-line): __fastcall so ecx = this.
    void TransferImpl(char* name, long* value, long* defaultValue);

    // The 2-arg wrapper we are reconstructing.
    void Transfer_long_(char* name, long* value);
};

void CPersistContext::Transfer_long_(char* name, long* value)
{
    long defaultValue = 0;
    TransferImpl(name, value, &defaultValue);
}