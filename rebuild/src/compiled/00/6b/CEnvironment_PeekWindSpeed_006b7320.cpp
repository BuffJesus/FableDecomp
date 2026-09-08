#include "fable_environment_retail.h"

struct CEnvironment_PeekWindSpeed : CEnvironmentRetail {
    float M() const;
};

float CEnvironment_PeekWindSpeed::M() const {
    return GameTickState.WindSpeed;
}
