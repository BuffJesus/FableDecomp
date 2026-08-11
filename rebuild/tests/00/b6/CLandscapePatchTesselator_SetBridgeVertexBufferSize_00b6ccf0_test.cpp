#include <stdio.h>
#include <string.h>

struct Buf24 {
    char d[24];
};

struct BridgeBuffer {
    long lastN;
    int  calls;
    void SetSize(long n, Buf24* out);
};

struct CLandscapePatchTesselator {
    char m_pad[0x14];
    BridgeBuffer m_bridge;

    void SetBridgeVertexBufferSize(long n);
};

void BridgeBuffer::SetSize(long n, Buf24* out)
{
    lastN = n;
    calls++;
    memset(out, 0, sizeof(*out));
    out->d[0] = (char)(n & 0xFF);
}

void CLandscapePatchTesselator::SetBridgeVertexBufferSize(long n)
{
    Buf24 tmp;
    m_bridge.SetSize(n, &tmp);
}

int main()
{
    CLandscapePatchTesselator t;
    memset(&t, 0, sizeof(t));

    t.SetBridgeVertexBufferSize(4096);
    if (t.m_bridge.lastN != 4096) { printf("FAIL n\n"); return 1; }
    if (t.m_bridge.calls != 1)    { printf("FAIL calls\n"); return 1; }

    t.SetBridgeVertexBufferSize(-1);
    if (t.m_bridge.lastN != -1)   { printf("FAIL n2\n"); return 1; }
    if (t.m_bridge.calls != 2)    { printf("FAIL calls2\n"); return 1; }

    printf("SETBRIDGE_OK\n");
    return 0;
}