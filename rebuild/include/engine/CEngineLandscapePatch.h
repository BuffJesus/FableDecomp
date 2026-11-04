#pragma once
#include "rebuild_abi.h"
// Retail patch allocation stride48 is independently used by UpdateForeground.
// The visibility-cleanup lane needs only its virtual deletion ABI.
class CEngineLandscapePatch {
public:
    virtual ~CEngineLandscapePatch();
    fable_u8 UnknownFields[0x44];
};
FABLE_STATIC_ASSERT(sizeof(CEngineLandscapePatch)==0x48);
