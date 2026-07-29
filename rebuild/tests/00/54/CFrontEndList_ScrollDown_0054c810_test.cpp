#include <cstdio>

#include "../../../src/compiled/00/54/CFrontEndList_ScrollDown_0054c810.cpp"

extern "C" const float FableFrontEndScrollDownMinusOne = -1.0f;
extern "C" const float FableFrontEndScrollDownInv255 =
    0.003921568859368563f;
extern "C" const float FableFrontEndScrollDownOne = 1.0f;
extern "C" const float FableFrontEndScrollDownZero = 0.0f;
extern "C" const float FableFrontEndScrollDown255 = 255.0f;

// Supply inert bodies for the unobserved retail vtable slots. Derived test
// doubles override only the recovered slots used by ScrollDown.
#define FE_DEFINE_REF_SLOT(Name) void FrontEndRefObject::Name() {}
FE_DEFINE_REF_SLOT(Slot00)
void FrontEndRefObject::Release() {}
#undef FE_DEFINE_REF_SLOT

#define FE_DEFINE_MANAGER_SLOT(Name) void CFrontEndManager::Name() {}
FE_SLOT_BLOCK_0(FE_DEFINE_MANAGER_SLOT)
FE_SLOT_BLOCK_1(FE_DEFINE_MANAGER_SLOT)
FE_DEFINE_MANAGER_SLOT(Slot20)
FE_DEFINE_MANAGER_SLOT(Slot21)
FE_DEFINE_MANAGER_SLOT(Slot22)
FE_DEFINE_MANAGER_SLOT(Slot23)
FE_DEFINE_MANAGER_SLOT(Slot24)
FE_DEFINE_MANAGER_SLOT(Slot25)
FE_DEFINE_MANAGER_SLOT(Slot26)
FE_DEFINE_MANAGER_SLOT(Slot27)
FE_DEFINE_MANAGER_SLOT(Slot29)
void CFrontEndManager::PlaySound(void*, int) {}
FrontEndDefPointer* CFrontEndManager::GetMiscDefinition(
    FrontEndDefPointer* output)
{
    return output;
}
#undef FE_DEFINE_MANAGER_SLOT

#define FE_DEFINE_COMPONENT_SLOT(Name) void CFrontEndComponent::Name() {}
FE_SLOT_BLOCK_0(FE_DEFINE_COMPONENT_SLOT)
FE_SLOT_BLOCK_1(FE_DEFINE_COMPONENT_SLOT)
FE_DEFINE_COMPONENT_SLOT(Slot21)
FE_DEFINE_COMPONENT_SLOT(Slot22)
FE_DEFINE_COMPONENT_SLOT(Slot23)
FE_DEFINE_COMPONENT_SLOT(Slot24)
FE_DEFINE_COMPONENT_SLOT(Slot25)
FE_DEFINE_COMPONENT_SLOT(Slot27)
FE_DEFINE_COMPONENT_SLOT(Slot28)
FE_DEFINE_COMPONENT_SLOT(Slot29)
FE_DEFINE_COMPONENT_SLOT(Slot2A)
FE_DEFINE_COMPONENT_SLOT(Slot2B)
FE_DEFINE_COMPONENT_SLOT(Slot2C)
FE_DEFINE_COMPONENT_SLOT(Slot2D)
FE_DEFINE_COMPONENT_SLOT(Slot2E)
FE_DEFINE_COMPONENT_SLOT(Slot2F)
FE_DEFINE_COMPONENT_SLOT(Slot31)
FE_DEFINE_COMPONENT_SLOT(Slot32)
FE_DEFINE_COMPONENT_SLOT(Slot33)
FE_DEFINE_COMPONENT_SLOT(Slot34)
FE_DEFINE_COMPONENT_SLOT(Slot35)
FE_DEFINE_COMPONENT_SLOT(Slot36)
void CFrontEndComponent::SetPosition(
    const FrontEndVector2&,
    float,
    bool)
{
}
void CFrontEndComponent::SetColour(
    const FrontEndColour&,
    float,
    bool)
{
}
void CFrontEndComponent::SetCurrentState(int)
{
}
FrontEndVector2& CFrontEndComponent::GetPosition(FrontEndVector2& output)
{
    return output;
}
FrontEndColour& CFrontEndComponent::GetColour(FrontEndColour& output)
{
    return output;
}
#undef FE_DEFINE_COMPONENT_SLOT

#define FE_DEFINE_LIST_SLOT(Name) void CFrontEndList_0054c810::Name() {}
FE_SLOT_BLOCK_0(FE_DEFINE_LIST_SLOT)
FE_SLOT_BLOCK_1(FE_DEFINE_LIST_SLOT)
FE_SLOT_BLOCK_2(FE_DEFINE_LIST_SLOT)
FE_DEFINE_LIST_SLOT(Slot30)
FE_DEFINE_LIST_SLOT(Slot31)
FE_DEFINE_LIST_SLOT(Slot32)
FE_DEFINE_LIST_SLOT(Slot33)
FE_DEFINE_LIST_SLOT(Slot34)
FE_DEFINE_LIST_SLOT(Slot35)
FE_DEFINE_LIST_SLOT(Slot37)
FE_DEFINE_LIST_SLOT(Slot38)
FE_DEFINE_LIST_SLOT(Slot39)
FE_DEFINE_LIST_SLOT(Slot3A)
FE_DEFINE_LIST_SLOT(Slot3B)
FE_DEFINE_LIST_SLOT(Slot3C)
FE_DEFINE_LIST_SLOT(Slot3D)
FE_DEFINE_LIST_SLOT(Slot3E)
FE_DEFINE_LIST_SLOT(Slot3F)
FE_SLOT_BLOCK_4(FE_DEFINE_LIST_SLOT)
FE_SLOT_BLOCK_5(FE_DEFINE_LIST_SLOT)
FE_SLOT_BLOCK_6(FE_DEFINE_LIST_SLOT)
FE_SLOT_BLOCK_7(FE_DEFINE_LIST_SLOT)
FE_SLOT_BLOCK_8(FE_DEFINE_LIST_SLOT)
FE_SLOT_BLOCK_9(FE_DEFINE_LIST_SLOT)
FE_DEFINE_LIST_SLOT(SlotA0)
FE_DEFINE_LIST_SLOT(SlotA1)
FE_DEFINE_LIST_SLOT(SlotA2)
FE_DEFINE_LIST_SLOT(SlotA3)
FE_DEFINE_LIST_SLOT(SlotA4)
FE_DEFINE_LIST_SLOT(SlotA5)
FE_DEFINE_LIST_SLOT(SlotA6)
FE_DEFINE_LIST_SLOT(SlotA7)
FE_DEFINE_LIST_SLOT(SlotA8)
FE_DEFINE_LIST_SLOT(SlotA9)
FE_DEFINE_LIST_SLOT(SlotAA)
float CFrontEndList_0054c810::GetUpdateTime()
{
    return 0.0f;
}
bool CFrontEndList_0054c810::IsScrolling()
{
    return false;
}
#undef FE_DEFINE_LIST_SLOT

static bool g_IsScrolling;
static float g_UpdateTime;
static int g_IsScrollingCalls;
static int g_UpdateTimeCalls;
static int g_GetInstanceCalls;
static int g_GetDefinitionCalls;
static int g_PlaySoundCalls;
static unsigned int g_PlaySoundOffset;
static int g_PlaySoundArgument;
static int g_ReleaseCalls;

class TestRefObject : public FrontEndRefObject
{
public:
    virtual void Release()
    {
        ++g_ReleaseCalls;
    }
};

class TestManager : public CFrontEndManager
{
public:
    virtual void PlaySound(void* sound, int argument);
    virtual FrontEndDefPointer* GetMiscDefinition(
        FrontEndDefPointer* output);
};

class TestList : public CFrontEndList_0054c810
{
public:
    virtual float GetUpdateTime()
    {
        ++g_UpdateTimeCalls;
        return g_UpdateTime;
    }

    virtual bool IsScrolling()
    {
        ++g_IsScrollingCalls;
        return g_IsScrolling;
    }
};

class TestComponent : public CFrontEndComponent
{
public:
    int id;
    FrontEndColour colour;
    FrontEndVector2 position;
    int stateHistory[4];
    int stateCount;
    FrontEndColour colourHistory[8];
    float colourTimeHistory[8];
    bool colourLinearHistory[8];
    int colourSetCount;
    FrontEndVector2 positionHistory[4];
    float positionTimeHistory[4];
    bool positionLinearHistory[4];
    int positionSetCount;
    int colourGetCount;
    int positionGetCount;

    void Reset(int componentId)
    {
        id = componentId;
        colour.red = (FrontEndByte)(10 + id);
        colour.green = (FrontEndByte)(20 + id);
        colour.blue = (FrontEndByte)(30 + id);
        colour.alpha = (FrontEndByte)(40 + id);
        position.x = (float)(100 + id);
        position.y = (float)(200 + id);
        stateCount = 0;
        colourSetCount = 0;
        positionSetCount = 0;
        colourGetCount = 0;
        positionGetCount = 0;
        for (int index = 0; index < 8; ++index)
        {
            colourTimeHistory[index] = 0.0f;
            colourLinearHistory[index] = false;
        }
        for (int index = 0; index < 4; ++index)
        {
            stateHistory[index] = 0;
            positionTimeHistory[index] = 0.0f;
            positionLinearHistory[index] = false;
        }
    }

    virtual void SetPosition(
        const FrontEndVector2& newPosition,
        float updateTime,
        bool linear)
    {
        int index = positionSetCount++;
        positionHistory[index] = newPosition;
        positionTimeHistory[index] = updateTime;
        positionLinearHistory[index] = linear;
        position = newPosition;
    }

    virtual void SetColour(
        const FrontEndColour& newColour,
        float updateTime,
        bool linear)
    {
        int index = colourSetCount++;
        colourHistory[index] = newColour;
        colourTimeHistory[index] = updateTime;
        colourLinearHistory[index] = linear;
        colour = newColour;
    }

    virtual void SetCurrentState(int state)
    {
        stateHistory[stateCount++] = state;
    }

    virtual FrontEndVector2& GetPosition(FrontEndVector2& output)
    {
        ++positionGetCount;
        output = position;
        return output;
    }

    virtual FrontEndColour& GetColour(FrontEndColour& output)
    {
        ++colourGetCount;
        output = colour;
        return output;
    }
};

static TestComponent g_Components[5];
static CFrontEndComponent* g_Children[5];
static TestList g_List;
static TestManager g_Manager;
static TestRefObject g_Definition;

void TestManager::PlaySound(void* sound, int argument)
{
    ++g_PlaySoundCalls;
    g_PlaySoundOffset =
        (unsigned int)((char*)sound - (char*)&g_Definition);
    g_PlaySoundArgument = argument;
}

FrontEndDefPointer* TestManager::GetMiscDefinition(
    FrontEndDefPointer* output)
{
    ++g_GetDefinitionCalls;
    output->object = &g_Definition;
    return output;
}

CFrontEndManager* __cdecl CFrontEndManager_GetInstance()
{
    ++g_GetInstanceCalls;
    return &g_Manager;
}

extern "C" void* __cdecl FableFrontEndScrollDownGetManager()
{
    return CFrontEndManager_GetInstance();
}

extern "C" __declspec(naked) int __cdecl
FableFrontEndScrollDownFtol2()
{
    __asm
    {
        sub esp, 8
        fnstcw word ptr [esp]
        mov ax, word ptr [esp]
        or ax, 0c00h
        mov word ptr [esp + 2], ax
        fldcw word ptr [esp + 2]
        fistp dword ptr [esp + 4]
        fldcw word ptr [esp]
        mov eax, dword ptr [esp + 4]
        add esp, 8
        ret
    }
}

static void ResetFixture(
    unsigned int childCount,
    unsigned int selectedIndex,
    bool scrolling,
    float updateTime,
    FrontEndByte alphaOffset,
    long referenceCount)
{
    for (int index = 0; index < 5; ++index)
    {
        g_Components[index].Reset(index);
        g_Children[index] = &g_Components[index];
    }

    g_List.selectedIndex = selectedIndex;
    g_List.childrenBegin = g_Children;
    g_List.childrenEnd = g_Children + childCount;
    g_List.alphaOffset = alphaOffset;
    g_Definition.references = referenceCount;

    g_IsScrolling = scrolling;
    g_UpdateTime = updateTime;
    g_IsScrollingCalls = 0;
    g_UpdateTimeCalls = 0;
    g_GetInstanceCalls = 0;
    g_GetDefinitionCalls = 0;
    g_PlaySoundCalls = 0;
    g_PlaySoundOffset = 0;
    g_PlaySoundArgument = 0;
    g_ReleaseCalls = 0;
}

static int CheckSound(
    unsigned int expectedOffset,
    long expectedReferences,
    int expectedReleaseCalls)
{
    if (g_GetInstanceCalls != 2 ||
        g_GetDefinitionCalls != 1 ||
        g_PlaySoundCalls != 1 ||
        g_PlaySoundOffset != expectedOffset ||
        g_PlaySoundArgument != 1)
    {
        std::printf("sound dispatch contract failed\n");
        return 1;
    }
    if (g_Definition.references != expectedReferences ||
        g_ReleaseCalls != expectedReleaseCalls)
    {
        std::printf("definition refcount contract failed\n");
        return 1;
    }
    return 0;
}

static int CheckNoComponentWrites()
{
    for (int index = 0; index < 5; ++index)
    {
        if (g_Components[index].stateCount != 0 ||
            g_Components[index].colourSetCount != 0 ||
            g_Components[index].positionSetCount != 0)
        {
            std::printf("unexpected component write\n");
            return 1;
        }
    }
    return 0;
}

static int CheckRecoveredLayout()
{
    if ((char*)&g_Definition.references - (char*)&g_Definition != 0x04 ||
        (char*)&g_List.selectedIndex - (char*)&g_List != 0x15c ||
        (char*)&g_List.childrenBegin - (char*)&g_List != 0x164 ||
        (char*)&g_List.childrenEnd - (char*)&g_List != 0x168 ||
        (char*)&g_List.alphaOffset - (char*)&g_List != 0x1f8)
    {
        std::printf("recovered object layout failed\n");
        return 1;
    }
    return 0;
}

int main()
{
    int failures = CheckRecoveredLayout();

    // Fewer than two children: invalid-move sound and immediate return.
    ResetFixture(0, 0, false, 0.5f, 0, 1);
    g_List.ScrollDown();
    failures += CheckSound(0x1a4, 0, 1);
    failures += CheckNoComponentWrites();

    // A retained definition decrements without invoking Release().
    ResetFixture(1, 0, false, 0.5f, 0, 2);
    g_List.ScrollDown();
    failures += CheckSound(0x1a4, 1, 0);
    failures += CheckNoComponentWrites();

    // A scrolling list rejects movement at its final child.
    ResetFixture(3, 2, true, 0.5f, 0, 1);
    g_List.ScrollDown();
    failures += CheckSound(0x1a4, 0, 1);
    failures += CheckNoComponentWrites();
    if (g_IsScrollingCalls != 1)
    {
        std::printf("end-boundary scrolling query failed\n");
        ++failures;
    }

    // Non-scrolling movement uses states 4/3, rotates colours, and maps -1 to
    // update time zero without translating positions.
    ResetFixture(3, 1, false, -1.0f, 0, 1);
    FrontEndColour originalColours[3];
    for (int index = 0; index < 3; ++index)
        originalColours[index] = g_Components[index].colour;
    g_List.ScrollDown();
    failures += CheckSound(0x194, 0, 1);
    if (g_List.selectedIndex != 2 ||
        g_Components[1].stateCount != 1 ||
        g_Components[1].stateHistory[0] != 4 ||
        g_Components[2].stateCount != 1 ||
        g_Components[2].stateHistory[0] != 3)
    {
        std::printf("selection state transition failed\n");
        ++failures;
    }
    if (g_UpdateTimeCalls != 1 ||
        g_Components[0].positionGetCount != 1 ||
        g_IsScrollingCalls != 4)
    {
        std::printf("sentinel/query call contract failed\n");
        ++failures;
    }
    for (int index = 0; index < 3; ++index)
    {
        int source = (index + 1) % 3;
        if (g_Components[index].colour.red != originalColours[source].red ||
            g_Components[index].colourTimeHistory[0] != 0.0f ||
            g_Components[index].colourLinearHistory[0] ||
            g_Components[index].positionSetCount != 0)
        {
            std::printf("non-scrolling colour rotation failed\n");
            ++failures;
            break;
        }
    }

    // A non-scrolling final child wraps to zero.
    ResetFixture(3, 2, false, 0.25f, 0, 1);
    g_List.ScrollDown();
    if (g_List.selectedIndex != 0 ||
        g_Components[2].stateHistory[0] != 4 ||
        g_Components[0].stateHistory[0] != 3 ||
        g_UpdateTimeCalls != 2)
    {
        std::printf("wrap-to-zero contract failed\n");
        ++failures;
    }

    // Scrolling movement translates every child and then applies the x87
    // distance-alpha pass. alphaOffset=255 makes the expected boundary exact:
    // selected child alpha 255, every non-selected child clamped to zero.
    ResetFixture(4, 0, true, 0.25f, 255, 1);
    g_List.ScrollDown();
    if (g_List.selectedIndex != 1 ||
        g_UpdateTimeCalls != 2 ||
        g_IsScrollingCalls != 5)
    {
        std::printf("scrolling selection/query contract failed\n");
        ++failures;
    }
    for (int index = 0; index < 4; ++index)
    {
        FrontEndByte expectedAlpha =
            (FrontEndByte)(index == 1 ? 255 : 0);
        if (g_Components[index].positionSetCount != 1 ||
            g_Components[index].position.x != 0.0f ||
            g_Components[index].position.y != -30.0f ||
            g_Components[index].positionTimeHistory[0] != 0.25f ||
            g_Components[index].positionLinearHistory[0] ||
            g_Components[index].colourSetCount != 2 ||
            g_Components[index].colour.red != 255 ||
            g_Components[index].colour.green != 255 ||
            g_Components[index].colour.blue != 255 ||
            g_Components[index].colour.alpha != expectedAlpha ||
            g_Components[index].colourTimeHistory[1] != 0.25f ||
            g_Components[index].colourLinearHistory[1])
        {
            std::printf("scrolling position/alpha contract failed child=%d\n", index);
            ++failures;
            break;
        }
    }

    // Exercise non-boundary alpha values too.  With falloff 64/255 and the
    // selection advancing to child one, the retail x87 path produces the
    // Retail's stored single-precision 1/255 constant and x87 truncation
    // produce the asymmetric byte ramp 190, 255, 190, 126, 62.
    ResetFixture(5, 0, true, 0.25f, 64, 1);
    g_List.ScrollDown();
    const FrontEndByte expectedRamp[5] = {190, 255, 190, 126, 62};
    if (g_List.selectedIndex != 1 ||
        g_UpdateTimeCalls != 2 ||
        g_IsScrollingCalls != 6)
    {
        std::printf("intermediate alpha query contract failed\n");
        ++failures;
    }
    for (int index = 0; index < 5; ++index)
    {
        if (g_Components[index].colour.alpha != expectedRamp[index])
        {
            std::printf(
                "intermediate alpha ramp failed child=%d got=%u\n",
                index,
                (unsigned int)g_Components[index].colour.alpha);
            ++failures;
            break;
        }
    }

    if (failures != 0)
    {
        std::printf("FRONTEND_0054c810_TEST FAIL count=%d\n", failures);
        return 1;
    }

    std::printf("FRONTEND_0054c810_TEST PASS\n");
    return 0;
}
