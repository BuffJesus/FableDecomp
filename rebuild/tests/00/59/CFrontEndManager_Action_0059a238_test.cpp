// Exact-candidate compile/link gate for CFrontEndManager::Action.
// The test intentionally does not fake a frontend route or bank load. It links
// the compiled candidate object against explicit seam stubs only to prove that
// the object has a complete link boundary.
#include <stdio.h>

class CFrontEndManager_0059a238
{
public:
    void Action(void* countedAction);
};

#define FABLE_ACTION_STUB(address) \
    extern "C" __declspec(naked) void FableActionCall_##address() { __asm ret }

FABLE_ACTION_STUB(009e1bc0)
FABLE_ACTION_STUB(00404440)
FABLE_ACTION_STUB(009c31e0)
FABLE_ACTION_STUB(009c3250)
FABLE_ACTION_STUB(00597b20)
FABLE_ACTION_STUB(0059b039)
FABLE_ACTION_STUB(0099b7d0)
FABLE_ACTION_STUB(0059b5d7)
FABLE_ACTION_STUB(0099ebf0)
FABLE_ACTION_STUB(009c95e0)
FABLE_ACTION_STUB(0099eae0)
FABLE_ACTION_STUB(0099bf30)
FABLE_ACTION_STUB(0099be70)
FABLE_ACTION_STUB(0099b510)
FABLE_ACTION_STUB(0040d2a0)
FABLE_ACTION_STUB(0040f0a0)
FABLE_ACTION_STUB(00407040)
FABLE_ACTION_STUB(00596cce)
FABLE_ACTION_STUB(0059686d)
FABLE_ACTION_STUB(00596763)
FABLE_ACTION_STUB(0042dbee)
FABLE_ACTION_STUB(00598463)
FABLE_ACTION_STUB(0040a740)
FABLE_ACTION_STUB(0059899a)
FABLE_ACTION_STUB(00851920)
FABLE_ACTION_STUB(00409b70)
FABLE_ACTION_STUB(00595845)
FABLE_ACTION_STUB(00597006)
FABLE_ACTION_STUB(00599d5c)
FABLE_ACTION_STUB(00851860)
FABLE_ACTION_STUB(005963db)
FABLE_ACTION_STUB(00597bf2)
FABLE_ACTION_STUB(009c7980)

extern "C" long FableActionGlobal_013ca818 = 0;
extern "C" long FableActionGlobal_013b871c = 0;
extern "C" const char FableActionString_01252a04[] = "";
extern "C" const char FableActionString_012529e4[] = "";
extern "C" const char FableActionString_0122d820[] = "";
extern "C" const char FableActionString_012529cc[] = "";
extern "C" const char FableActionString_01236a1c[] = "";

int main()
{
    printf("FRONTEND_ACTION_0059A238_TEST PASS\n");
    return 0;
}
