#include <cstdio>
#include <cstring>

#include "../../../src/compiled/00/49/CFrontendGameComponent_GetMaxVisibleProfileNumber_00494520.cpp"

static void SetProfiles(
    CFrontendGameComponent_00494520* component,
    void** storage,
    unsigned long count,
    unsigned long first_visible)
{
    component->profiles_begin = storage;
    component->profiles_end = storage + count;
    component->profiles_capacity = storage + count;
    component->first_visible_profile = first_visible;
}

int main()
{
    int failures = 0;
    CFrontendGameComponent_00494520 component;
    void* profiles[12];

    std::memset(&component, 0, sizeof(component));

    SetProfiles(&component, profiles, 10, 3);
    if (component.GetMaxVisibleProfileNumber() != 7)
    {
        std::printf("unclamped remaining-profile count failed\n");
        ++failures;
    }

    SetProfiles(&component, profiles, 12, 3);
    if (component.GetMaxVisibleProfileNumber() != 8)
    {
        std::printf("eight-row clamp failed\n");
        ++failures;
    }

    SetProfiles(&component, profiles, 12, 12);
    if (component.GetMaxVisibleProfileNumber() != 0)
    {
        std::printf("empty tail failed\n");
        ++failures;
    }

    // Retail's unsigned `ja` also clamps an invalid underflowed tail to eight.
    SetProfiles(&component, profiles, 4, 5);
    if (component.GetMaxVisibleProfileNumber() != 8)
    {
        std::printf("unsigned underflow clamp failed\n");
        ++failures;
    }

    if (component.profiles_begin != profiles ||
        component.profiles_end != profiles + 4 ||
        component.first_visible_profile != 5)
    {
        std::printf("non-mutating layout query failed\n");
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "FRONTEND_00494520_TEST FAIL count=%d\n",
            failures);
        return 1;
    }

    std::printf("FRONTEND_00494520_TEST PASS\n");
    return 0;
}
