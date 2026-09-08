#include "fable_environment_retail.h"

struct CEnvironment_PeekMoonVector : CEnvironmentRetail {
    const CEnvironmentVector* M() const;
};

const CEnvironmentVector* CEnvironment_PeekMoonVector::M() const {
    return &GameTickState.MoonVector;
}
