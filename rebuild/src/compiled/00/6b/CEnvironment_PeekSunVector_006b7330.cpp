#include "fable_environment_retail.h"

struct CEnvironment_PeekSunVector : CEnvironmentRetail {
    const CEnvironmentVector* M() const;
};

const CEnvironmentVector* CEnvironment_PeekSunVector::M() const {
    return &GameTickState.SunVector;
}
