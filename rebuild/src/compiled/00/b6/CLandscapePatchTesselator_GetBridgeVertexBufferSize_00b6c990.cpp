// CLandscapePatchTesselator::GetBridgeVertexBufferSize @ 0x00b6c990
// Returns pointer-difference element count for a 24-byte-element buffer.

struct BridgeVertex { char pad[24]; };  // sizeof == 24

struct CLandscapePatchTesselator {
    char           pad0[0x14];     // 0x00 .. 0x13
    BridgeVertex*  m_begin;        // 0x14
    BridgeVertex*  m_end;          // 0x18

    long GetBridgeVertexBufferSize();
};

long CLandscapePatchTesselator::GetBridgeVertexBufferSize()
{
    return (long)(this->m_end - this->m_begin);
}