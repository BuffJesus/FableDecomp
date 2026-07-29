#include <cstdio>
#include <cstring>

#include "../../../src/compiled/00/4b/CQuestManager_DeactivateAllQuests_004b4450.cpp"

struct QuestListNode_004b4450
{
    QuestListNode_004b4450* next;
    QuestListNode_004b4450* previous;
    void* quest;
    long* countedControl;
};

static volatile int g_CallCount;
static void* volatile g_QuestsSeen[3];
static void* volatile g_ControlsSeen[3];
static volatile long g_CountsSeen[3];
static volatile int g_PreserveSeen[3];

extern "C" void __fastcall
CQuestManager_DeactivateQuest_004b4450(
    void* manager,
    void*,
    void* quest,
    void* countedControl,
    int preserveQuest)
{
    const int call = g_CallCount++;
    g_QuestsSeen[call] = quest;
    g_ControlsSeen[call] = countedControl;
    g_CountsSeen[call] =
        countedControl != 0 ? *(long*)countedControl : -1;
    g_PreserveSeen[call] = preserveQuest;

    QuestListNode_004b4450* sentinel =
        *(QuestListNode_004b4450**)((unsigned char*)manager + 0x38);
    QuestListNode_004b4450* first = sentinel->next;
    sentinel->next = first->next;
    first->next->previous = sentinel;

    // The by-value counted argument is destroyed by the real callee.
    if (countedControl != 0)
        --*(long*)countedControl;
}

static void ResetProbe()
{
    g_CallCount = 0;
    for (int index = 0; index != 3; ++index)
    {
        g_QuestsSeen[index] = 0;
        g_ControlsSeen[index] = 0;
        g_CountsSeen[index] = -2;
        g_PreserveSeen[index] = -1;
    }
}

int main()
{
    unsigned char manager[0x3c];
    QuestListNode_004b4450 sentinel;
    QuestListNode_004b4450 first;
    QuestListNode_004b4450 second;
    long firstCount = 5;
    int firstQuest;
    int secondQuest;
    std::memset(manager, 0, sizeof(manager));

    sentinel.next = &sentinel;
    sentinel.previous = &sentinel;
    sentinel.quest = 0;
    sentinel.countedControl = 0;
    *(QuestListNode_004b4450**)(manager + 0x38) = &sentinel;

    typedef void (__fastcall *DeactivateAllFunction)(void*, void*);
    DeactivateAllFunction volatile deactivateAll =
        &CQuestManager_DeactivateAllQuests_004b4450;

    ResetProbe();
    deactivateAll(manager, 0);
    int failures = 0;
    if (g_CallCount != 0 || sentinel.next != &sentinel)
        ++failures;

    first.next = &second;
    first.previous = &sentinel;
    first.quest = &firstQuest;
    first.countedControl = &firstCount;
    second.next = &sentinel;
    second.previous = &first;
    second.quest = &secondQuest;
    second.countedControl = 0;
    sentinel.next = &first;
    sentinel.previous = &second;

    ResetProbe();
    deactivateAll(manager, 0);

    if (g_CallCount != 2 ||
        g_QuestsSeen[0] != &firstQuest ||
        g_QuestsSeen[1] != &secondQuest ||
        g_ControlsSeen[0] != &firstCount ||
        g_ControlsSeen[1] != 0 ||
        g_CountsSeen[0] != 6 ||
        g_CountsSeen[1] != -1 ||
        g_PreserveSeen[0] != 0 ||
        g_PreserveSeen[1] != 0 ||
        firstCount != 5 ||
        sentinel.next != &sentinel ||
        sentinel.previous != &sentinel)
    {
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "DEACTIVATE_ALL_QUESTS_004b4450_TEST FAIL count=%d calls=%d\n",
            failures,
            g_CallCount);
        return 1;
    }

    std::printf("DEACTIVATE_ALL_QUESTS_004b4450_TEST PASS\n");
    return 0;
}
