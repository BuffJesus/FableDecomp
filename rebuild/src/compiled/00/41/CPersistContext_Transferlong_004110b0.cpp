// CPersistContext::Transfer<long>  0x004110b0
struct CPersistContext {
    void Inner(char* pName, long* pData, long* pDefault);
    void Transfer_long_(char* pName, long* pData);
};

void CPersistContext::Transfer_long_(char* pName, long* pData)
{
    long dflt = 0;
    Inner(pName, pData, &dflt);
}