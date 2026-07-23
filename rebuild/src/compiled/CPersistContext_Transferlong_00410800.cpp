struct CPersistContext {
    void Transfer(int a, int b);
    void TransferImpl(int a, int b, long* pv);
};

void CPersistContext::Transfer(int a, int b)
{
    long v = 0;
    TransferImpl(a, b, &v);
}