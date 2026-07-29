#include <cstdio>
#include <cstring>

#include "../../../src/compiled/00/49/CFrontendGameComponent_GetMaxVisibleProfileNumber_00494770.cpp"

static void SetEntries(
    CFrontendGameComponent_00494770* component,
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
    CFrontendGameComponent_00494770 component;
    void* entries[12];

    std::memset(&component, 0, sizeof(component));

    SetEntries(&component, entries, 9, 2);
    if (component.GetMaxVisibleProfileNumber() != 7)
    {
        std::printf("save-list remaining count failed\n");
        ++failures;
    }

    SetEntries(&component, entries, 12, 1);
    if (component.GetMaxVisibleProfileNumber() != 8)
    {
        std::printf("save-list eight-row clamp failed\n");
        ++failures;
    }

    SetEntries(&component, entries, 12, 12);
    if (component.GetMaxVisibleProfileNumber() != 0)
    {
        std::printf("save-list empty tail failed\n");
        ++failures;
    }

    SetEntries(&component, entries, 5, 6);
    if (component.GetMaxVisibleProfileNumber() != 8)
    {
        std::printf("save-list unsigned underflow clamp failed\n");
        ++failures;
    }

    if (component.entries_begin != entries ||
        component.entries_end != entries + 5 ||
        component.first_visible_entry != 6)
    {
        std::printf("save-list non-mutating query failed\n");
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "FRONTEND_00494770_TEST FAIL count=%d\n",
            failures);
        return 1;
    }

    std::printf("FRONTEND_00494770_TEST PASS\n");
    return 0;
}
