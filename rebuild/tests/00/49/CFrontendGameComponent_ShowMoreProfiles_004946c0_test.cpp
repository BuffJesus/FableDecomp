#include <cstdio>
#include <cstring>

#include "../../../src/compiled/00/49/CFrontendGameComponent_ShowMoreProfiles_004946c0.cpp"

static void SetEntries(
    CFrontendGameComponent_004946c0* component,
    void** storage,
    unsigned long count,
    unsigned long first_visible)
{
    component->entries_begin = storage;
    component->entries_end = storage + count;
    component->entries_capacity = storage + count;
    component->first_visible_entry = first_visible;
}

int main()
{
    int failures = 0;
    CFrontendGameComponent_004946c0 component;
    void* entries[16];

    std::memset(&component, 0, sizeof(component));

    SetEntries(&component, entries, 8, 2);
    component.ShowMoreProfiles();
    if (component.first_visible_entry != 2)
    {
        std::printf("single-page no-op failed\n");
        ++failures;
    }

    SetEntries(&component, entries, 13, 0);
    component.ShowMoreProfiles();
    if (component.first_visible_entry != 8)
    {
        std::printf("second-list page advance failed\n");
        ++failures;
    }

    component.ShowMoreProfiles();
    if (component.first_visible_entry != 0)
    {
        std::printf("second-list past-end wrap failed\n");
        ++failures;
    }

    SetEntries(&component, entries, 16, 8);
    component.ShowMoreProfiles();
    if (component.first_visible_entry != 16)
    {
        std::printf("second-list equal-end retention failed\n");
        ++failures;
    }

    if (component.entries_begin != entries ||
        component.entries_end != entries + 16 ||
        component.entries_capacity != entries + 16)
    {
        std::printf("second-list vector preservation failed\n");
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "FRONTEND_004946c0_TEST FAIL count=%d\n",
            failures);
        return 1;
    }

    std::printf("FRONTEND_004946c0_TEST PASS\n");
    return 0;
}
