// Candidate-bound fixture for CSBIAgents::Display (retail 0x00A211C0, empty: ret 4): links against
// the landed object of CSBIAgents_Display_00a211c0.cpp, calls the real member (qualified, non-virtual)
// and checks it returns without touching the object.
#include <stdio.h>
#include <string.h>
#include "fable_steering_behaviour.h"
static unsigned char storage[0x40];
static unsigned char expected[0x40];
int main()
{
    memset(storage, 0x5A, sizeof(storage));
    memcpy(expected, storage, sizeof(storage));
    CSBIAgents* object = reinterpret_cast<CSBIAgents*>(storage);
    object->CSBIAgents::Display(0);
    object->CSBIAgents::Display(reinterpret_cast<const CSteeringAgent*>(storage));
    if (memcmp(storage, expected, sizeof(storage)) == 0) { printf("AUTO_TINY_00a211c0_TEST PASS\n"); return 0; }
    printf("AUTO_TINY_00a211c0_TEST FAIL\n");
    return 1;
}
