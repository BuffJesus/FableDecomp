#include <cstdio>
#include <cstring>

#include "../../../src/compiled/00/49/CFrontendGameComponent_ShowMoreProfiles_00494560.cpp"

static void SetProfiles(
    CFrontendGameComponent_00494560* component,
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
    CFrontendGameComponent_00494560 component;
    void* profiles[16];

    std::memset(&component, 0, sizeof(component));

    SetProfiles(&component, profiles, 8, 3);
    component.ShowMoreProfiles();
    if (component.first_visible_profile != 3)
    {
        std::printf("single-page no-op failed\n");
        ++failures;
    }

    SetProfiles(&component, profiles, 12, 0);
    component.ShowMoreProfiles();
    if (component.first_visible_profile != 8)
    {
        std::printf("page advance failed\n");
        ++failures;
    }

    component.ShowMoreProfiles();
    if (component.first_visible_profile != 0)
    {
        std::printf("past-end wrap failed\n");
        ++failures;
    }

    SetProfiles(&component, profiles, 16, 8);
    component.ShowMoreProfiles();
    if (component.first_visible_profile != 16)
    {
        std::printf("equal-end retention failed\n");
        ++failures;
    }

    if (component.profiles_begin != profiles ||
        component.profiles_end != profiles + 16 ||
        component.profiles_capacity != profiles + 16)
    {
        std::printf("profile vector preservation failed\n");
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "FRONTEND_00494560_TEST FAIL count=%d\n",
            failures);
        return 1;
    }

    std::printf("FRONTEND_00494560_TEST PASS\n");
    return 0;
}
