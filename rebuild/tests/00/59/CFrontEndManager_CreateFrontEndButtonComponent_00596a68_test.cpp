#include <cstdio>
#include <cstring>

#include "../../../src/compiled/00/59/CFrontEndManager_CreateFrontEndButtonComponent_00596a68.cpp"

extern "C" const char FrontEndEmptyString_00596a68[] = "";
extern "C" const char FrontEndUIButton_00596a68[] = "UI_BUTTON";
extern "C" const char FrontEndUIButtonText_00596a68[] = "UI_BUTTON_TEXT";
extern "C" const char FrontEndUITextButton_00596a68[] = "UI_TEXT_BUTTON";
extern "C" const char FrontEndUIFrontEndButton_00596a68[] =
    "UI_FRONTEND_BUTTON";

enum TestComponentId_00596a68
{
    COMPONENT_BACKGROUND_00596a68 = 1,
    COMPONENT_ACTION_00596a68 = 2,
    COMPONENT_TEXT_BUTTON_00596a68 = 3,
    COMPONENT_FRONTEND_00596a68 = 4
};

struct TestComponent_00596a68
{
    void** vtable;
    int identifier;
};

static void* g_ComponentVtable[145];
static TestComponent_00596a68 g_Background;
static TestComponent_00596a68 g_Action;
static TestComponent_00596a68 g_TextButton;
static TestComponent_00596a68 g_FrontEnd;
static CFrontEndManager_00596a68 g_Manager;
static CButtonState_00596a68 g_States[6];
static const char* g_CreateNames[4];
static int g_CreateCalls;
static int g_CharConstructCalls;
static const char* g_ConstructedNames[4];
static int g_CharDestroyCalls;
static int g_CharEqualsCalls;
static int g_ShareCalls;
static int g_ConfigureCalls;
static CWideString_00596a68* g_ConfiguredText;
static bool g_ConfiguredEnabled;
static int g_ConfiguredConstant;
static int g_AddChildCalls;
static int g_AddParents[3];
static int g_AddChildren[3];
static int g_StateKeys[5];
static int g_StateCalls;
static int g_LastStateKey;
static int g_ReleaseCalls;
static int g_ReleaseOrder[4];

static int ComponentId(CComponent_00596a68* component)
{
    return ((TestComponent_00596a68*)component)->identifier;
}

extern "C" void __fastcall TestAddChild_00596a68(
    CComponent_00596a68* component,
    void*,
    CCountedComponent_00596a68* child)
{
    g_AddParents[g_AddChildCalls] = ComponentId(component);
    g_AddChildren[g_AddChildCalls] = ComponentId(child->object);
    ++g_AddChildCalls;
}

extern "C" void* __fastcall TestGetState_00596a68(
    CComponent_00596a68*,
    void*,
    int* key)
{
    g_LastStateKey = *key;
    g_StateKeys[g_StateCalls++] = *key;
    return &g_LastStateKey;
}

extern "C" void __fastcall TestConfigureAction_00596a68(
    CComponent_00596a68*,
    void*,
    CWideString_00596a68* text,
    bool enabled,
    int constant)
{
    ++g_ConfigureCalls;
    g_ConfiguredText = text;
    g_ConfiguredEnabled = enabled;
    g_ConfiguredConstant = constant;
}

static TestComponent_00596a68* ComponentForName(const char* name)
{
    if (std::strcmp(name, FrontEndUIButtonText_00596a68) == 0)
        return &g_Action;
    if (std::strcmp(name, FrontEndUITextButton_00596a68) == 0)
        return &g_TextButton;
    if (std::strcmp(name, "CUSTOM_FRONTEND") == 0 ||
        std::strcmp(name, FrontEndUIFrontEndButton_00596a68) == 0)
    {
        return &g_FrontEnd;
    }
    return &g_Background;
}

extern "C" CComponent_00596a68* __fastcall
FrontEndCreateComponent_00596a68(
    CFrontEndManager_00596a68* manager,
    void*,
    const CCharString_00596a68* definition)
{
    if (manager != &g_Manager)
        return 0;
    g_CreateNames[g_CreateCalls++] = definition->data;
    return (CComponent_00596a68*)ComponentForName(definition->data);
}

extern "C" void __fastcall FrontEndCountedReset_00596a68(
    CCountedComponent_00596a68* counted,
    void*,
    CComponent_00596a68* component)
{
    counted->object = component;
    counted->pointerInfo = (void*)0x11110001;
}

extern "C" void __fastcall FrontEndCharConstruct_00596a68(
    CCharString_00596a68* string,
    void*,
    const char* text,
    int length)
{
    if (length == -1 && g_CharConstructCalls < 4)
        g_ConstructedNames[g_CharConstructCalls] = text;
    ++g_CharConstructCalls;
    string->data = text;
}

extern "C" void __fastcall FrontEndCharDestroy_00596a68(
    CCharString_00596a68*,
    void*)
{
    ++g_CharDestroyCalls;
}

extern "C" void __fastcall FrontEndCountedConstruct_00596a68(
    CCountedComponent_00596a68* counted,
    void*,
    CComponent_00596a68* component)
{
    counted->object = component;
    counted->pointerInfo = (void*)0x22220002;
}

extern "C" void __fastcall FrontEndCountedShare_00596a68(
    CCountedComponent_00596a68* counted,
    void*,
    CComponent_00596a68* component,
    void* pointerInfo)
{
    ++g_ShareCalls;
    counted->object = component;
    counted->pointerInfo = pointerInfo;
}

extern "C" bool __fastcall FrontEndCharEquals_00596a68(
    const char* stringData,
    void*,
    const char* text)
{
    ++g_CharEqualsCalls;
    return std::strcmp(stringData, text) == 0;
}

extern "C" CButtonState_00596a68* __fastcall
FrontEndStateTransfer_00596a68(void*, void*)
{
    return &g_States[g_LastStateKey];
}

static void RecordRelease(CCountedComponent_00596a68* counted)
{
    g_ReleaseOrder[g_ReleaseCalls++] = ComponentId(counted->object);
    counted->object = 0;
    counted->pointerInfo = 0;
}

extern "C" void __fastcall FrontEndCountedReleaseA_00596a68(
    CCountedComponent_00596a68* counted,
    void*)
{
    RecordRelease(counted);
}

extern "C" void __fastcall FrontEndCountedReleaseB_00596a68(
    CCountedComponent_00596a68* counted,
    void*)
{
    RecordRelease(counted);
}

static void ResetFixture()
{
    for (int index = 0; index < 145; ++index)
        g_ComponentVtable[index] = 0;
    g_ComponentVtable[0xec / 4] = (void*)&TestAddChild_00596a68;
    g_ComponentVtable[0x148 / 4] = (void*)&TestGetState_00596a68;
    g_ComponentVtable[0x240 / 4] = (void*)&TestConfigureAction_00596a68;

    g_Background.vtable = g_ComponentVtable;
    g_Background.identifier = COMPONENT_BACKGROUND_00596a68;
    g_Action.vtable = g_ComponentVtable;
    g_Action.identifier = COMPONENT_ACTION_00596a68;
    g_TextButton.vtable = g_ComponentVtable;
    g_TextButton.identifier = COMPONENT_TEXT_BUTTON_00596a68;
    g_FrontEnd.vtable = g_ComponentVtable;
    g_FrontEnd.identifier = COMPONENT_FRONTEND_00596a68;

    for (int index = 0; index < 6; ++index)
        g_States[index].width = -1.0f;

    g_CreateCalls = 0;
    g_CharConstructCalls = 0;
    g_CharDestroyCalls = 0;
    g_CharEqualsCalls = 0;
    g_ShareCalls = 0;
    g_ConfigureCalls = 0;
    g_ConfiguredText = 0;
    g_ConfiguredEnabled = false;
    g_ConfiguredConstant = 0;
    g_AddChildCalls = 0;
    g_StateCalls = 0;
    g_LastStateKey = -1;
    g_ReleaseCalls = 0;
}

static int CheckComposition(
    CComponent_00596a68* result,
    CWideString_00596a68* text,
    float width,
    bool enabled)
{
    int failures = 0;
    const int expectedParents[3] =
    {
        COMPONENT_TEXT_BUTTON_00596a68,
        COMPONENT_TEXT_BUTTON_00596a68,
        COMPONENT_FRONTEND_00596a68
    };
    const int expectedChildren[3] =
    {
        COMPONENT_ACTION_00596a68,
        COMPONENT_BACKGROUND_00596a68,
        COMPONENT_TEXT_BUTTON_00596a68
    };
    const int expectedKeys[5] = {0, 1, 3, 4, 5};
    const int expectedReleases[4] =
    {
        COMPONENT_TEXT_BUTTON_00596a68,
        COMPONENT_ACTION_00596a68,
        COMPONENT_ACTION_00596a68,
        COMPONENT_BACKGROUND_00596a68
    };

    if (result != (CComponent_00596a68*)&g_FrontEnd ||
        g_ShareCalls != 1 ||
        g_ConfigureCalls != 1 ||
        g_ConfiguredText != text ||
        g_ConfiguredEnabled != enabled ||
        g_ConfiguredConstant != 1)
    {
        std::printf("action/result contract failed\n");
        ++failures;
    }
    if (g_AddChildCalls != 3)
    {
        std::printf("component composition count failed\n");
        ++failures;
    }
    for (int index = 0; index < 3; ++index)
    {
        if (g_AddParents[index] != expectedParents[index] ||
            g_AddChildren[index] != expectedChildren[index])
        {
            std::printf("component composition order failed\n");
            ++failures;
            break;
        }
    }
    if (g_StateCalls != 5)
    {
        std::printf("state lookup count failed\n");
        ++failures;
    }
    for (int index = 0; index < 5; ++index)
    {
        if (g_StateKeys[index] != expectedKeys[index] ||
            g_States[expectedKeys[index]].width != width)
        {
            std::printf("state width contract failed\n");
            ++failures;
            break;
        }
    }
    if (g_States[2].width != -1.0f || g_ReleaseCalls != 4)
    {
        std::printf("state/release boundary failed\n");
        ++failures;
    }
    for (int index = 0; index < 4; ++index)
    {
        if (g_ReleaseOrder[index] != expectedReleases[index])
        {
            std::printf("counted release order failed\n");
            ++failures;
            break;
        }
    }
    return failures;
}

int main()
{
    int failures = 0;
    CWideString_00596a68 text;
    text.data = (void*)0x12345678;

    // Provided definitions bypass default construction but retain the fixed
    // action and text-button definitions.
    ResetFixture();
    CCharString_00596a68 customFrontEnd = {"CUSTOM_FRONTEND"};
    CCharString_00596a68 customButton = {"CUSTOM_BUTTON"};
    CComponent_00596a68* result =
        g_Manager.CreateFrontEndButtonComponent(
            text,
            42.5f,
            customFrontEnd,
            customButton,
            true);
    failures += CheckComposition(result, &text, 42.5f, true);
    if (g_CreateCalls != 4 ||
        std::strcmp(g_CreateNames[0], "CUSTOM_BUTTON") != 0 ||
        std::strcmp(g_CreateNames[1], FrontEndUIButtonText_00596a68) != 0 ||
        std::strcmp(g_CreateNames[2], FrontEndUITextButton_00596a68) != 0 ||
        std::strcmp(g_CreateNames[3], "CUSTOM_FRONTEND") != 0 ||
        g_CharConstructCalls != 2 ||
        g_CharDestroyCalls != 2 ||
        g_CharEqualsCalls != 2)
    {
        std::printf("provided-definition routing failed\n");
        ++failures;
    }

    // Null backing strings take the explicit default paths without invoking
    // the non-null string equality helper.
    ResetFixture();
    CCharString_00596a68 nullFrontEnd = {0};
    CCharString_00596a68 nullButton = {0};
    result = g_Manager.CreateFrontEndButtonComponent(
        text,
        17.25f,
        nullFrontEnd,
        nullButton,
        false);
    failures += CheckComposition(result, &text, 17.25f, false);
    if (g_CreateCalls != 4 ||
        std::strcmp(g_CreateNames[0], FrontEndUIButton_00596a68) != 0 ||
        std::strcmp(g_CreateNames[3], FrontEndUIFrontEndButton_00596a68) != 0 ||
        g_CharConstructCalls != 4 ||
        g_CharDestroyCalls != 4 ||
        g_CharEqualsCalls != 0)
    {
        std::printf("default-definition routing failed\n");
        ++failures;
    }

    // Non-null empty strings reach the equality helper and select the same
    // defaults as the null-backed representation.
    ResetFixture();
    CCharString_00596a68 emptyFrontEnd = {FrontEndEmptyString_00596a68};
    CCharString_00596a68 emptyButton = {FrontEndEmptyString_00596a68};
    result = g_Manager.CreateFrontEndButtonComponent(
        text,
        9.0f,
        emptyFrontEnd,
        emptyButton,
        true);
    failures += CheckComposition(result, &text, 9.0f, true);
    if (g_CreateCalls != 4 ||
        std::strcmp(g_CreateNames[0], FrontEndUIButton_00596a68) != 0 ||
        std::strcmp(g_CreateNames[3], FrontEndUIFrontEndButton_00596a68) != 0 ||
        g_CharConstructCalls != 4 ||
        g_CharDestroyCalls != 4 ||
        g_CharEqualsCalls != 2)
    {
        std::printf("empty-definition routing failed\n");
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "FRONTEND_00596a68_TEST FAIL count=%d\n",
            failures);
        return 1;
    }

    std::printf("FRONTEND_00596a68_TEST PASS\n");
    return 0;
}
