// Real-object ABI/behavior fixture for
// NUISystem::CFrontEndManager::DeactivateAllEditBoxes @ 0x005952D8.

#include <cstdio>
#include <cstring>

extern "C" void __fastcall
CFrontEndManager_DeactivateAllEditBoxes_005952d8(
    void* manager,
    void*,
    void* component);

struct ComponentProbe;

struct ChildEntry
{
    ComponentProbe* component;
    unsigned long metadata;
};

struct ChildrenProbe
{
    ChildEntry* first;
    ChildEntry* last;
    ChildEntry* end;
};

struct ComponentProbe
{
    void** vtable;
    long type;
    long active;
    int typeCalls;
    int deactivateCalls;
    int childrenCalls;
    ChildrenProbe children;
};

static void* g_vtable[0x25c / 4];
static void* g_expectedStack;
static void* g_topComponent;
static int g_topCalls;
static int g_totalTypeCalls;
static int g_totalDeactivateCalls;
static int g_totalChildrenCalls;

extern "C" void** __fastcall
FableFrontEndDeactivateEditBoxesStackTop(void* stack, void*)
{
    if (stack == g_expectedStack)
        ++g_topCalls;
    return &g_topComponent;
}

static long __fastcall
ComponentGetType(void* object, void*)
{
    ComponentProbe* component = (ComponentProbe*)object;
    ++component->typeCalls;
    ++g_totalTypeCalls;
    return component->type;
}

static void __fastcall
ComponentSetActive(void* object, void*, long active)
{
    ComponentProbe* component = (ComponentProbe*)object;
    component->active = active;
    ++component->deactivateCalls;
    ++g_totalDeactivateCalls;
}

static ChildrenProbe* __fastcall
ComponentGetChildren(void* object, void*)
{
    ComponentProbe* component = (ComponentProbe*)object;
    ++component->childrenCalls;
    ++g_totalChildrenCalls;
    return &component->children;
}

static void Initialise(
    ComponentProbe& component,
    long type,
    ChildEntry* children,
    unsigned long count)
{
    std::memset(&component, 0, sizeof(component));
    component.vtable = g_vtable;
    component.type = type;
    component.active = 1;
    component.children.first = children;
    component.children.last = children + count;
    component.children.end = component.children.last;
}

static void Reset(unsigned char* manager)
{
    std::memset(manager, 0, 0x40);
    std::memset(g_vtable, 0, sizeof(g_vtable));
    g_vtable[0x104 / 4] = (void*)ComponentGetType;
    g_vtable[0x258 / 4] = (void*)ComponentSetActive;
    g_vtable[0xd4 / 4] = (void*)ComponentGetChildren;
    g_expectedStack = manager + 0x20;
    g_topComponent = 0;
    g_topCalls = 0;
    g_totalTypeCalls = 0;
    g_totalDeactivateCalls = 0;
    g_totalChildrenCalls = 0;
}

static bool RunEmptyTop()
{
    unsigned char manager[0x40];
    Reset(manager);
    CFrontEndManager_DeactivateAllEditBoxes_005952d8(manager, 0, 0);
    return g_topCalls == 1 &&
        g_totalTypeCalls == 0 &&
        g_totalDeactivateCalls == 0 &&
        g_totalChildrenCalls == 0;
}

static bool RunTree()
{
    unsigned char manager[0x40];
    ComponentProbe root;
    ComponentProbe editA;
    ComponentProbe container;
    ComponentProbe editB;
    ChildEntry rootChildren[2];
    ChildEntry containerChildren[1];
    Reset(manager);

    rootChildren[0].component = &editA;
    rootChildren[0].metadata = 1;
    rootChildren[1].component = &container;
    rootChildren[1].metadata = 2;
    containerChildren[0].component = &editB;
    containerChildren[0].metadata = 3;
    Initialise(root, 1, rootChildren, 2);
    Initialise(editA, 0x25, 0, 0);
    Initialise(container, 2, containerChildren, 1);
    Initialise(editB, 0x25, 0, 0);
    g_topComponent = &root;

    CFrontEndManager_DeactivateAllEditBoxes_005952d8(manager, 0, 0);

    return g_topCalls == 1 &&
        g_totalTypeCalls == 4 &&
        g_totalDeactivateCalls == 2 &&
        g_totalChildrenCalls == 10 &&
        root.active == 1 &&
        container.active == 1 &&
        editA.active == 0 &&
        editB.active == 0 &&
        editA.deactivateCalls == 1 &&
        editB.deactivateCalls == 1;
}

static bool RunExplicit()
{
    unsigned char manager[0x40];
    ComponentProbe edit;
    Reset(manager);
    Initialise(edit, 0x25, 0, 0);

    CFrontEndManager_DeactivateAllEditBoxes_005952d8(
        manager, 0, &edit);

    return g_topCalls == 0 &&
        g_totalTypeCalls == 1 &&
        g_totalDeactivateCalls == 1 &&
        g_totalChildrenCalls == 1 &&
        edit.active == 0;
}

int main()
{
    const bool empty = RunEmptyTop();
    const bool tree = RunTree();
    const bool explicitRoot = RunExplicit();
    if (!empty || !tree || !explicitRoot)
    {
        std::printf(
            "FSE2_005952d8_TEST FAIL empty=%d tree=%d explicit=%d\n",
            empty ? 1 : 0,
            tree ? 1 : 0,
            explicitRoot ? 1 : 0);
        return 1;
    }
    std::puts("FSE2_005952d8_TEST PASS");
    return 0;
}
