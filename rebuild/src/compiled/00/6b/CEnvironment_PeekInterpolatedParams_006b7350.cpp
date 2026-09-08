#include "fable_environment_retail.h"

struct CEnvironment_PeekInterpolatedParams : CEnvironmentRetail {
    const void* M() const;
};

const void* CEnvironment_PeekInterpolatedParams::M() const {
    return InterpolatedEnvironmentParams;
}
