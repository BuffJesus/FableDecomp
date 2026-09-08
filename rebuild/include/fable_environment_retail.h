#ifndef FABLE_ENVIRONMENT_RETAIL_H
#define FABLE_ENVIRONMENT_RETAIL_H

#include <stddef.h>
#include "rebuild_abi.h"

// The retail CEnvironmentGameTickState is smaller than the Ego_r PDB type,
// but the surviving members retain their original names and order.  Keep the
// retail ABI adjustment here instead of repeating anonymous offsets in every
// accessor.
#pragma pack(push, 1)
struct CEnvironmentVector {
    float X;
    float Y;
    float Z;
};

struct CEnvironmentGameTickStateRetail {
    unsigned char _unrecovered_0x000[0xbb8];
    float WindSpeed;                         // retail CEnvironment +0xc10
    CEnvironmentVector WindDirection;        // retail CEnvironment +0xc14
    float ThemeWindSpeed;
    CEnvironmentVector ThemeWindDirection;
    CEnvironmentVector SunVector;            // retail CEnvironment +0xc30
    CEnvironmentVector MoonVector;           // retail CEnvironment +0xc3c
    unsigned char _unrecovered_0xbf0[0x2e];
};

struct CEnvironmentRetail {
    unsigned char _unrecovered_0x0000[0x58];
    CEnvironmentGameTickStateRetail GameTickState;
    CEnvironmentGameTickStateRetail PrevGameTickState;
    unsigned char InterpolatedEnvironmentParams[0xc4];
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CEnvironmentVector) == 0xc);
FABLE_STATIC_ASSERT(sizeof(CEnvironmentGameTickStateRetail) == 0xc1e);
FABLE_STATIC_ASSERT(offsetof(CEnvironmentGameTickStateRetail, WindSpeed) == 0xbb8);
FABLE_STATIC_ASSERT(offsetof(CEnvironmentGameTickStateRetail, WindDirection) == 0xbbc);
FABLE_STATIC_ASSERT(offsetof(CEnvironmentGameTickStateRetail, SunVector) == 0xbd8);
FABLE_STATIC_ASSERT(offsetof(CEnvironmentGameTickStateRetail, MoonVector) == 0xbe4);
FABLE_STATIC_ASSERT(offsetof(CEnvironmentRetail, GameTickState) == 0x58);
FABLE_STATIC_ASSERT(offsetof(CEnvironmentRetail, InterpolatedEnvironmentParams) == 0x1894);

#endif // FABLE_ENVIRONMENT_RETAIL_H
