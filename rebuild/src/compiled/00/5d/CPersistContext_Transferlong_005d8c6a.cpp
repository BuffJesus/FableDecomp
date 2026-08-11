#pragma optimize("s",on)
// CPersistContext::Transfer<long>(char* name, long* value)
// retail 0x005d8c6a: zero-inits a local default and forwards to the
// generic transfer helper as (name, value, &default0).

struct CPersistContext {
    void Transfer_long_(char* name, long* value);
};

// generic transfer helper at retail 0x005d8978 (name, value, &default)
void __stdcall CPersistContext_TransferHelper(char* name, long* value, long* def);

void CPersistContext::Transfer_long_(char* name, long* value)
{
    long def = 0;
    CPersistContext_TransferHelper(name, value, &def);
}