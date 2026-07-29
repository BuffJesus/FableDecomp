#include <cstdio>
#include <cstring>

#include "../../../src/compiled/00/49/CFrontendGameComponent_ShowMoreProfiles_004947b0.cpp"

static void SetEntries(
    CFrontendGameComponent_004947b0* component,
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
    CFrontendGameComponent_004947b0 component;
    void* entries[16];

    std::memset(&component, 0, sizeof(component));

    SetEntries(&component, entries, 8, 5);
    component.ShowMoreProfiles();
    if (component.first_visible_entry != 5)
    {
        std::printf("save-list single-page no-op failed\n");
        ++failures;
    }

    SetEntries(&component, entries, 14, 0);
    component.ShowMoreProfiles();
    if (component.first_visible_entry != 8)
    {
        std::printf("save-list page advance failed\n");
        ++failures;
    }

    component.ShowMoreProfiles();
    if (component.first_visible_entry != 0)
    {
        std::printf("save-list past-end wrap failed\n");
        ++failures;
    }

    SetEntries(&component, entries, 16, 8);
    component.ShowMoreProfiles();
    if (component.first_visible_entry != 16)
    {
        std::printf("save-list equal-end retention failed\n");
        ++failures;
    }

    if (component.entries_begin != entries ||
        component.entries_end != entries + 16 ||
        component.entries_capacity != entries + 16)
    {
        std::printf("save-list vector preservation failed\n");
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "FRONTEND_004947b0_TEST FAIL count=%d\n",
            failures);
        return 1;
    }

    std::printf("FRONTEND_004947b0_TEST PASS\n");
    return 0;
}
