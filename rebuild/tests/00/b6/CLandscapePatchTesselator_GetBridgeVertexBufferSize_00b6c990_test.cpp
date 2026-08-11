#include <cstdio>

struct BridgeVertex { char pad[24]; };

struct CLandscapePatchTesselator {
    char           pad0[0x14];
    BridgeVertex*  m_begin;
    BridgeVertex*  m_end;
    long GetBridgeVertexBufferSize();
};

long CLandscapePatchTesselator::GetBridgeVertexBufferSize()
{
    return (long)(this->m_end - this->m_begin);
}

int main()
{
    BridgeVertex arr[10];
    CLandscapePatchTesselator t;
    t.m_begin = arr;
    t.m_end   = arr + 7;
    long n = t.GetBridgeVertexBufferSize();

    CLandscapePatchTesselator z;
    z.m_begin = arr;
    z.m_end   = arr;
    long zero = z.GetBridgeVertexBufferSize();

    if (n == 7 && zero == 0) {
        printf("BRIDGEVBSIZE_OK n=%ld zero=%ld\n", n, zero);
        return 0;
    }
    printf("FAIL n=%ld zero=%ld\n", n, zero);
    return 1;
}