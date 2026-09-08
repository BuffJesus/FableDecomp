#include "fable_environment_retail.h"

struct CEnvironment_PeekWindDirection : CEnvironmentRetail {
    const CEnvironmentVector* M() const;
};

const CEnvironmentVector* CEnvironment_PeekWindDirection::M() const {
    return &GameTickState.WindDirection;
}
