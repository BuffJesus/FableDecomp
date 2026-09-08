#include "engine/CLandscapePatchTesselator.h"

struct CBridgeVertexBufferResult {
    unsigned char data[24];
};

struct CBridgeVertexBufferMethods {
    void SetSize(long size, CBridgeVertexBufferResult* result);
};

struct CLandscapePatchTesselator_Methods : CLandscapePatchTesselator {
    void SetBridgeVertexBufferSize(long size);
};

void CLandscapePatchTesselator_Methods::SetBridgeVertexBufferSize(long size)
{
    CBridgeVertexBufferResult result;
    CBridgeVertexBufferMethods* buffer = (CBridgeVertexBufferMethods*)&this->m_begin;
    buffer->SetSize(size, &result);
}
