// Candidate-bound fixture for CSBCCorridor::GetType (retail 0xA21F90): links against the landed object of
// CSBCCorridor_GetType_00a21f90.cpp and calls the real member through a qualified (non-virtual) call; the
// body never reads this, so a zeroed buffer stands in for the object.
#include <stdio.h>
#include "fable_steering_behaviour.h"
static unsigned char storage[0x40];
int main()
{
    CSBCCorridor* object = reinterpret_cast<CSBCCorridor*>(storage);
    const CSB::ESB_TYPE type = object->CSBCCorridor::GetType();
    if (type == CSB::ESBC_CORRIDOR && static_cast<int>(type) == 3) { printf("AUTO_TINY_00a21f90_TEST PASS\n"); return 0; }
    printf("AUTO_TINY_00a21f90_TEST FAIL %d\n", static_cast<int>(type));
    return 1;
}
