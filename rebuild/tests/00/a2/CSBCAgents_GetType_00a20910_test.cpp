// Candidate-bound fixture for CSBCAgents::GetType (retail 0xA20910): links against the landed object of
// CSBCAgents_GetType_00a20910.cpp and calls the real member through a qualified (non-virtual) call; the
// body never reads this, so a zeroed buffer stands in for the object.
#include <stdio.h>
#include "fable_steering_behaviour.h"
static unsigned char storage[0x40];
int main()
{
    CSBCAgents* object = reinterpret_cast<CSBCAgents*>(storage);
    const CSB::ESB_TYPE type = object->CSBCAgents::GetType();
    if (type == CSB::ESBC_AGENTS && static_cast<int>(type) == 4) { printf("AUTO_TINY_00a20910_TEST PASS\n"); return 0; }
    printf("AUTO_TINY_00a20910_TEST FAIL %d\n", static_cast<int>(type));
    return 1;
}
