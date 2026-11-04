// Candidate-bound fixture for CSBIAgents::GetType (retail 0xA211D0): links against the landed object of
// CSBIAgents_GetType_00a211d0.cpp and calls the real member through a qualified (non-virtual) call; the
// body never reads this, so a zeroed buffer stands in for the object.
#include <stdio.h>
#include "fable_steering_behaviour.h"
static unsigned char storage[0x40];
int main()
{
    CSBIAgents* object = reinterpret_cast<CSBIAgents*>(storage);
    const CSB::ESB_TYPE type = object->CSBIAgents::GetType();
    if (type == CSB::ESBI_AGENTS && static_cast<int>(type) == 2) { printf("AUTO_TINY_00a211d0_TEST PASS\n"); return 0; }
    printf("AUTO_TINY_00a211d0_TEST FAIL %d\n", static_cast<int>(type));
    return 1;
}
