#include <cstdio>
#include <cstring>

#include "../../../src/compiled/00/49/CFrontendGameComponent_SetQuit_00494370.cpp"

int main()
{
    int failures = 0;
    CFrontendGameComponent_00494370 component;

    std::memset(&component, 0xa5, sizeof(component));
    component.quit_requested = 0;
    component.SetQuit();

    if (component.quit_requested != 1)
    {
        std::printf("quit request transition failed\n");
        ++failures;
    }

    int index;
    for (index = 0; index != 8; ++index)
    {
        if (component.padding_0000[index] != 0xa5)
        {
            std::printf("unrelated byte mutation failed index=%d\n", index);
            ++failures;
            break;
        }
    }

    component.SetQuit();
    if (component.quit_requested != 1)
    {
        std::printf("quit request idempotence failed\n");
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "FRONTEND_00494370_TEST FAIL count=%d\n",
            failures);
        return 1;
    }

    std::printf("FRONTEND_00494370_TEST PASS\n");
    return 0;
}
