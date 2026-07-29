#include <cstdio>
#include <cstring>

#include "../../../src/compiled/00/59/CFrontEndManager_RefreshAvailableProfilesForDelete_00597006.cpp"

extern "C" const char FrontEndDeleteListDefinition_00597006[] =
    "UI_FRONTEND_LIST_FOR_PROFILES_FOR_DELETE";
extern "C" const char FrontEndEmptyDefinition_00597006[] = "";
extern "C" const char FrontEndDeleteButtonDefinition_00597006[] =
    "UI_FRONTEND_BUTTON_FOR_PROFILES_LIST";
extern "C" const float FrontEndProfileSpacing_00597006 = 30.0f;

enum ComponentId_00597006
{
    ROOT_00597006 = 1,
    OLD_LIST_00597006 = 2,
    NEW_LIST_00597006 = 3,
    BUTTON0_00597006 = 10,
    BUTTON1_00597006 = 11
};

struct TestComponent_00597006
{
    void** vtable;
    int identifier;
};

static void* g_Vtable[72];
static TestComponent_00597006 g_Root;
static TestComponent_00597006 g_OldList;
static TestComponent_00597006 g_NewList;
static TestComponent_00597006 g_Buttons[2];
static CFrontEndManager_00597006 g_Manager;
static CWideString_00597006 g_Profiles[2];
static CComponent_00597006* g_RootPointer;
static CComponent_00597006* g_OldListResult;
static CAction_00597006 g_Actions[2];
static CActionParamString_00597006 g_Parameters[2];
static int g_ProfileCount;
static int g_GetNamesCalls;
static int g_LookupCalls;
static int g_LookupKey;
static int g_FindCalls;
static int g_ClearCalls;
static int g_CreateListCalls;
static int g_RootAddCalls;
static int g_RootActivateCalls;
static int g_CreateButtonCalls;
static CWideString_00597006* g_ButtonProfiles[2];
static float g_ButtonOffsets[2];
static int g_AllocateCalls;
static unsigned int g_AllocationSizes[4];
static int g_ActionConstructCalls;
static int g_ParamConstructCalls;
static CWideString_00597006* g_ParamProfiles[2];
static int g_ParamAssignCalls;
static unsigned int g_AssignedActionTypes[2];
static CActionParamString_00597006* g_AssignedParams[2];
static int g_ButtonActionCalls108;
static int g_ButtonActionCalls11c;
static int g_ListAddCalls;
static int g_ListAddedButtons[2];
static int g_ActionReleaseCalls;
static int g_ButtonReleaseCalls;
static int g_ListReleaseCalls;
static int g_VectorTidyCalls;

static int ComponentId(CComponent_00597006* component)
{
    return ((TestComponent_00597006*)component)->identifier;
}

extern "C" CComponent_00597006* __fastcall TestFindChild_00597006(
    CComponent_00597006* component,
    void*,
    CCharString_00597006 definition)
{
    ++g_FindCalls;
    if (ComponentId(component) != ROOT_00597006 ||
        std::strcmp(
            definition.data,
            FrontEndDeleteListDefinition_00597006) != 0)
    {
        return 0;
    }
    return g_OldListResult;
}

extern "C" void __fastcall TestAddChild_00597006(
    CComponent_00597006* component,
    void*,
    CCountedComponent_00597006* child)
{
    if (ComponentId(component) == ROOT_00597006 &&
        ComponentId(child->object) == NEW_LIST_00597006)
    {
        ++g_RootAddCalls;
    }
}

extern "C" void __fastcall TestClearOld_00597006(
    CComponent_00597006* component,
    void*,
    unsigned int,
    unsigned int)
{
    if (ComponentId(component) == OLD_LIST_00597006)
        ++g_ClearCalls;
}

extern "C" void __fastcall TestActivateRoot_00597006(
    CComponent_00597006* component,
    void*)
{
    if (ComponentId(component) == ROOT_00597006)
        ++g_RootActivateCalls;
}

extern "C" void __fastcall TestButtonAction108_00597006(
    CComponent_00597006*,
    void*,
    CCountedComponent_00597006* action)
{
    if (action->object != 0)
        ++g_ButtonActionCalls108;
}

extern "C" void __fastcall TestButtonAction11c_00597006(
    CComponent_00597006*,
    void*,
    CCountedComponent_00597006* action)
{
    if (action->object != 0)
        ++g_ButtonActionCalls11c;
}

extern "C" void __fastcall TestListAddButton_00597006(
    CComponent_00597006* component,
    void*,
    CCountedComponent_00597006* button)
{
    if (ComponentId(component) == NEW_LIST_00597006)
    {
        g_ListAddedButtons[g_ListAddCalls++] = ComponentId(button->object);
    }
}

extern "C" void __fastcall FrontEndGetProfileNames_00597006(
    CFrontEndManager_00597006* manager,
    void*,
    CProfileVector_00597006* output)
{
    ++g_GetNamesCalls;
    if (manager == &g_Manager)
    {
        output->begin = g_Profiles;
        output->end = g_Profiles + g_ProfileCount;
        output->capacity = g_Profiles + 2;
    }
}

extern "C" void* __cdecl FrontEndComponentManagerGet_00597006()
{
    return &g_Manager;
}

extern "C" CComponent_00597006** __fastcall
FrontEndUsedLookup_00597006(
    CUsedComponentMap_00597006* used,
    void*,
    const int* key)
{
    ++g_LookupCalls;
    g_LookupKey = *key;
    if (used != &g_Manager.usedComponents)
        return 0;
    return &g_RootPointer;
}

extern "C" void __fastcall FrontEndCharConstruct_00597006(
    CCharString_00597006* string,
    void*,
    const char* text,
    int)
{
    string->data = text;
}

extern "C" void __fastcall FrontEndTemporaryListInit_00597006(
    void* value,
    void*)
{
    *(unsigned int*)value = 0;
}

extern "C" void __fastcall FrontEndTemporaryPairConstruct_00597006(
    void* value,
    void*,
    const void*)
{
    *(unsigned int*)value = 1;
}

extern "C" void __fastcall FrontEndTemporaryListDestroy_00597006(
    void*,
    void*)
{
}

extern "C" CComponent_00597006* __fastcall
FrontEndManagerCreateComponent_00597006(
    void* manager,
    void*,
    const CCharString_00597006* definition,
    bool enabled)
{
    if (manager == &g_Manager &&
        !enabled &&
        std::strcmp(
            definition->data,
            FrontEndDeleteListDefinition_00597006) == 0)
    {
        ++g_CreateListCalls;
    }
    return (CComponent_00597006*)&g_NewList;
}

extern "C" void __fastcall FrontEndCountedConstruct_00597006(
    CCountedComponent_00597006* counted,
    void*,
    CComponent_00597006* component)
{
    counted->object = component;
    counted->pointerInfo = (void*)0x12340000;
}

extern "C" void __fastcall FrontEndCharDestroy_00597006(
    CCharString_00597006*,
    void*)
{
}

extern "C" void __fastcall FrontEndCountedShare_00597006(
    CCountedComponent_00597006* counted,
    void*,
    CComponent_00597006* component,
    void* pointerInfo)
{
    counted->object = component;
    counted->pointerInfo = pointerInfo;
}

extern "C" CComponent_00597006* __fastcall
FrontEndCreateDeleteButton_00597006(
    CFrontEndManager_00597006* manager,
    void*,
    CWideString_00597006* profile,
    float offset,
    const CCharString_00597006* buttonDefinition,
    const CCharString_00597006* emptyDefinition,
    bool enabled)
{
    int index = g_CreateButtonCalls++;
    g_ButtonProfiles[index] = profile;
    g_ButtonOffsets[index] = offset;
    if (manager != &g_Manager ||
        enabled ||
        std::strcmp(
            buttonDefinition->data,
            FrontEndDeleteButtonDefinition_00597006) != 0 ||
        std::strcmp(
            emptyDefinition->data,
            FrontEndEmptyDefinition_00597006) != 0)
    {
        return 0;
    }
    return (CComponent_00597006*)&g_Buttons[index];
}

extern "C" void* __cdecl FrontEndAllocate_00597006(unsigned int bytes)
{
    int call = g_AllocateCalls++;
    g_AllocationSizes[call] = bytes;
    if ((call & 1) == 0)
        return &g_Actions[call / 2];
    return &g_Parameters[call / 2];
}

extern "C" CAction_00597006* __fastcall FrontEndActionConstruct_00597006(
    CAction_00597006* action,
    void*)
{
    ++g_ActionConstructCalls;
    action->type = 0;
    return action;
}

extern "C" void __fastcall FrontEndActionCountedConstruct_00597006(
    CCountedComponent_00597006* counted,
    void*,
    CComponent_00597006* action)
{
    counted->object = action;
    counted->pointerInfo = (void*)0x56780000;
}

extern "C" CActionParamString_00597006* __fastcall
FrontEndActionParamConstruct_00597006(
    CActionParamString_00597006* parameter,
    void*,
    CWideString_00597006* profile)
{
    g_ParamProfiles[g_ParamConstructCalls++] = profile;
    return parameter;
}

extern "C" void __fastcall FrontEndActionParamAssign_00597006(
    void* actionParameterSlot,
    void*,
    CActionParamString_00597006** parameter)
{
    CAction_00597006* action =
        (CAction_00597006*)((char*)actionParameterSlot - 4);
    g_AssignedActionTypes[g_ParamAssignCalls] = action->type;
    g_AssignedParams[g_ParamAssignCalls] = *parameter;
    ++g_ParamAssignCalls;
}

extern "C" void __fastcall FrontEndActionRelease_00597006(
    CCountedComponent_00597006*,
    void*)
{
    ++g_ActionReleaseCalls;
}

extern "C" void __fastcall FrontEndComponentRelease_00597006(
    CCountedComponent_00597006* counted,
    void*)
{
    if (counted->object == (CComponent_00597006*)&g_NewList)
        ++g_ListReleaseCalls;
    else
        ++g_ButtonReleaseCalls;
}

extern "C" void __fastcall FrontEndListRelease_00597006(
    CCountedComponent_00597006*,
    void*)
{
    ++g_ListReleaseCalls;
}

extern "C" void __fastcall FrontEndProfileVectorTidy_00597006(
    CProfileVector_00597006*,
    void*)
{
    ++g_VectorTidyCalls;
}

static void ResetFixture(int profileCount, bool oldListExists)
{
    for (int index = 0; index < 72; ++index)
        g_Vtable[index] = 0;
    g_Vtable[0x0c / 4] = (void*)&TestFindChild_00597006;
    g_Vtable[0xac / 4] = (void*)&TestActivateRoot_00597006;
    g_Vtable[0xec / 4] = (void*)&TestAddChild_00597006;
    g_Vtable[0xf0 / 4] = (void*)&TestListAddButton_00597006;
    g_Vtable[0xf8 / 4] = (void*)&TestClearOld_00597006;
    g_Vtable[0x108 / 4] = (void*)&TestButtonAction108_00597006;
    g_Vtable[0x11c / 4] = (void*)&TestButtonAction11c_00597006;

    g_Root.vtable = g_Vtable;
    g_Root.identifier = ROOT_00597006;
    g_OldList.vtable = g_Vtable;
    g_OldList.identifier = OLD_LIST_00597006;
    g_NewList.vtable = g_Vtable;
    g_NewList.identifier = NEW_LIST_00597006;
    for (int index = 0; index < 2; ++index)
    {
        g_Buttons[index].vtable = g_Vtable;
        g_Buttons[index].identifier = BUTTON0_00597006 + index;
        g_Profiles[index].data = (void*)(0x1000 + index);
    }

    g_RootPointer = (CComponent_00597006*)&g_Root;
    g_OldListResult = oldListExists
        ? (CComponent_00597006*)&g_OldList
        : 0;
    g_ProfileCount = profileCount;
    g_GetNamesCalls = 0;
    g_LookupCalls = 0;
    g_LookupKey = 0;
    g_FindCalls = 0;
    g_ClearCalls = 0;
    g_CreateListCalls = 0;
    g_RootAddCalls = 0;
    g_RootActivateCalls = 0;
    g_CreateButtonCalls = 0;
    g_AllocateCalls = 0;
    g_ActionConstructCalls = 0;
    g_ParamConstructCalls = 0;
    g_ParamAssignCalls = 0;
    g_ButtonActionCalls108 = 0;
    g_ButtonActionCalls11c = 0;
    g_ListAddCalls = 0;
    g_ActionReleaseCalls = 0;
    g_ButtonReleaseCalls = 0;
    g_ListReleaseCalls = 0;
    g_VectorTidyCalls = 0;
}

int main()
{
    int failures = 0;

    // Two profiles exercise old-list clearing, per-profile ownership, action
    // setup, 30-unit spacing, and list population.
    ResetFixture(2, true);
    long count = g_Manager.RefreshAvailableProfilesForDelete();
    if (count != 2 ||
        g_GetNamesCalls != 1 ||
        g_LookupCalls != 1 ||
        g_LookupKey != 0x15 ||
        g_FindCalls != 1 ||
        g_ClearCalls != 1 ||
        g_CreateListCalls != 1 ||
        g_RootAddCalls != 1 ||
        g_RootActivateCalls != 1 ||
        g_CreateButtonCalls != 2 ||
        g_ButtonProfiles[0] != &g_Profiles[0] ||
        g_ButtonProfiles[1] != &g_Profiles[1] ||
        g_ButtonOffsets[0] != 0.0f ||
        g_ButtonOffsets[1] != 30.0f)
    {
        std::printf("profile list construction failed\n");
        ++failures;
    }
    if (g_AllocateCalls != 4 ||
        g_AllocationSizes[0] != 0x10 ||
        g_AllocationSizes[1] != 0x0c ||
        g_AllocationSizes[2] != 0x10 ||
        g_AllocationSizes[3] != 0x0c ||
        g_ActionConstructCalls != 2 ||
        g_ParamConstructCalls != 2 ||
        g_ParamProfiles[0] != &g_Profiles[0] ||
        g_ParamProfiles[1] != &g_Profiles[1] ||
        g_ParamAssignCalls != 2 ||
        g_AssignedActionTypes[0] != 0xd7 ||
        g_AssignedActionTypes[1] != 0xd7)
    {
        std::printf("delete action ownership failed\n");
        ++failures;
    }
    if (g_ButtonActionCalls108 != 2 ||
        g_ButtonActionCalls11c != 2 ||
        g_ListAddCalls != 2 ||
        g_ListAddedButtons[0] != BUTTON0_00597006 ||
        g_ListAddedButtons[1] != BUTTON1_00597006 ||
        g_ActionReleaseCalls != 2 ||
        g_ButtonReleaseCalls != 2 ||
        g_ListReleaseCalls != 2 ||
        g_VectorTidyCalls != 1)
    {
        std::printf("component attach/release contract failed\n");
        ++failures;
    }

    // An empty profile vector still replaces/attaches and activates the list,
    // releases both counted owners, and tidies the vector.
    ResetFixture(0, false);
    count = g_Manager.RefreshAvailableProfilesForDelete();
    if (count != 0 ||
        g_ClearCalls != 0 ||
        g_CreateButtonCalls != 0 ||
        g_AllocateCalls != 0 ||
        g_RootAddCalls != 1 ||
        g_RootActivateCalls != 1 ||
        g_ListReleaseCalls != 2 ||
        g_VectorTidyCalls != 1)
    {
        std::printf("empty-profile boundary failed\n");
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "FRONTEND_00597006_TEST FAIL count=%d\n",
            failures);
        return 1;
    }

    std::printf("FRONTEND_00597006_TEST PASS\n");
    return 0;
}
