#ifndef FABLE_DISPLAY_VIEWPORT_H
#define FABLE_DISPLAY_VIEWPORT_H

#include "rebuild_abi.h"

struct C2DBoxF
{
public:
    float left;
    float top;
    float right;
    float bottom;
};

struct C2DBoxI
{
public:
    fable_i32 left;
    fable_i32 top;
    fable_i32 right;
    fable_i32 bottom;
};

class CDisplayManager
{
public:
    void SetViewport(const C2DBoxF& viewport);

    // Compact link seam for the retail integer overload. The complete retail
    // endpoint clamps/caches the viewport, calls D3D9, then notifies the
    // process-wide shader manager. A standalone renderer can supply this
    // member with the same thiscall ABI and omit only that singleton callback.
    void SetIntegerViewportEndpoint(const C2DBoxI& viewport);
};

typedef char FableC2DBoxFSizeMustBe16[
    sizeof(C2DBoxF) == 0x10 ? 1 : -1];
typedef char FableC2DBoxISizeMustBe16[
    sizeof(C2DBoxI) == 0x10 ? 1 : -1];

#endif
