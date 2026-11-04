// Candidate-bound fixture for CSBGoal::GetType (retail 0xA20860): links against the landed object of
// CSBGoal_GetType_00a20860.cpp and calls the real member through a qualified (non-virtual) call; the
// body never reads this, so a zeroed buffer stands in for the object.
#include <stdio.h>
#include "fable_steering_behaviour.h"
static unsigned char storage[0x40];
int main()
{
    CSBGoal* object = reinterpret_cast<CSBGoal*>(storage);
    const CSB::ESB_TYPE type = object->CSBGoal::GetType();
    if (type == CSB::ESB_GOAL && static_cast<int>(type) == 1) { printf("AUTO_TINY_00a20860_TEST PASS\n"); return 0; }
    printf("AUTO_TINY_00a20860_TEST FAIL %d\n", static_cast<int>(type));
    return 1;
}
