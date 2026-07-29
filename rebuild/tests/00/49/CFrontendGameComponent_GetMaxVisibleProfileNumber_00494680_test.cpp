#include <cstdio>
#include <cstring>

#include "../../../src/compiled/00/49/CFrontendGameComponent_GetMaxVisibleProfileNumber_00494680.cpp"

static void SetEntries(
    CFrontendGameComponent_00494680* component,
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
    CFrontendGameComponent_00494680 component;
    void* entries[12];

    std::memset(&component, 0, sizeof(component));

    SetEntries(&component, entries, 11, 4);
    if (component.GetMaxVisibleProfileNumber() != 7)
    {
        std::printf("unclamped remaining entry count failed\n");
        ++failures;
    }

    SetEntries(&component, entries, 12, 2);
    if (component.GetMaxVisibleProfileNumber() != 8)
    {
        std::printf("eight-row clamp failed\n");
        ++failures;
    }

    SetEntries(&component, entries, 12, 12);
    if (component.GetMaxVisibleProfileNumber() != 0)
    {
        std::printf("empty tail failed\n");
        ++failures;
    }

    SetEntries(&component, entries, 3, 4);
    if (component.GetMaxVisibleProfileNumber() != 8)
    {
        std::printf("unsigned underflow clamp failed\n");
        ++failures;
    }

    if (component.entries_begin != entries ||
        component.entries_end != entries + 3 ||
        component.first_visible_entry != 4)
    {
        std::printf("non-mutating list query failed\n");
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "FRONTEND_00494680_TEST FAIL count=%d\n",
            failures);
        return 1;
    }

    std::printf("FRONTEND_00494680_TEST PASS\n");
    return 0;
}
