#ifndef FABLE_ENGINE_C2DBOXI_H
#define FABLE_ENGINE_C2DBOXI_H

#include "rebuild_abi.h"

struct C2DCoordI {
    long X;
    long Y;
};

struct C2DBoxI {
    long TLX;
    long TLY;
    long BRX;
    long BRY;

    int ContainsPoint(const C2DCoordI* point) const;
    void ExtendBordersBy(long amount);
    void ExtendToContain(const C2DCoordI& point);
    void ValidateOrientationAndFlip();
};

FABLE_STATIC_ASSERT(sizeof(C2DCoordI) == 0x8);
FABLE_STATIC_ASSERT(sizeof(C2DBoxI) == 0x10);

#endif
