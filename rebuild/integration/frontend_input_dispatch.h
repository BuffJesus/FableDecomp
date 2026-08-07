#ifndef FABLE_FRONTEND_INPUT_DISPATCH_H
#define FABLE_FRONTEND_INPUT_DISPATCH_H

// Retail frontend input vocabulary recovered from Fable.exe:
//
//   CNewFrontendGameComponent::Input @ 0x0042E3EE
//       mouse-wheel input type 0x0E, positive delta -> action 0x24
//       mouse-wheel input type 0x0E, negative delta -> action 0x25
//       both directions use the strict +/-0.0001 threshold
//   CList::ProcessEvent @ 0x0053673B
//       event 0 -> ScrollUp, event 1 -> ScrollDown
//   CClickable::ProcessEvent @ 0x0055AD60
//       event 0x1A -> left-clicked, event 0x1C -> left-unclicked
//
// This is the small platform-neutral boundary used by the visual checkpoint.
// Win32 supplies the raw signed wheel word; the checkpoint then uses these
// retail action/event IDs rather than inventing a direction protocol.
enum FableRetailFrontendAction
{
    FableRetailFrontendActionWheelUp = 0x24,
    FableRetailFrontendActionWheelDown = 0x25
};

enum FableRetailFrontendListEvent
{
    FableRetailFrontendListEventUp = 0,
    FableRetailFrontendListEventDown = 1
};

// CFrontEndManager::Action @ 0x0059A238.  These are manager action IDs,
// not local UI command numbers; callers must stop at this boundary until the
// corresponding retail manager object/callback is linked.
enum FableRetailFrontendManagerAction
{
    FableRetailFrontendManagerActionBack = 0x56,
    FableRetailFrontendManagerActionDeleteConfirm = 0xD6,
    FableRetailFrontendManagerActionDeleteRow = 0xD7,
    FableRetailFrontendManagerActionDeleteListRefresh = 0xFA,
    FableRetailFrontendManagerActionLoadProfile = 0x124,
    FableRetailFrontendManagerActionNewProfile = 0x125,
    FableRetailFrontendManagerActionKeyboardConfirm = 0x126,
    FableRetailFrontendManagerActionKeyboardCancel = 0x127
};

enum FableRetailFrontendClickableEvent
{
    FableRetailFrontendClickableLeftClicked = 0x1A,
    FableRetailFrontendClickableLeftUnclicked = 0x1C
};

// CMouseDX::GetExclusiveModeDeviceMouseEvent @ 0x00C551C0 publishes the
// signed wheel payload as movement in thousandths. CInputEvent stores that
// float before CNewFrontendGameComponent::Input applies its +/-0.0001 test.
// Keep this conversion separate so callers cannot pass a raw WHEEL_DELTA to
// the retail classifier.
inline float FableRetailFrontendMouseWheelMovementFromRaw(
    short rawWheelDelta)
{
    return static_cast<float>(rawWheelDelta) * 0.001f;
}

// Exact condition results consumed by CList::ProcessEvent @ 0x0053673B.
// These correspond to the already-evaluated retail branches.  They remain
// condition results because the packed-byte temporary in the decompilation
// does not provide reliable semantic names for the underlying UI fields.
struct FableRetailFrontendListProcessContext
{
    bool viewportConditionPasses;
    bool hoverConditionPasses;
    bool managerActionConditionPasses;
    bool componentConditionPasses;
};

// NUISystem::CManager::ProcessEvent @ 0x0055CB10 (the slot-0 entry used by
// CNewFrontendGameComponent::Input through the CManager singleton at
// 0x0041E5F2).  The native dispatcher prefers the current component.  If no
// current component exists, it snapshots the registered component list,
// evaluates each component's vtable +8 condition, and calls vtable +4 for
// every component whose condition passes.
typedef bool (*FableRetailFrontendManagerEventCondition)(
    void* component,
    unsigned int event);
typedef void (*FableRetailFrontendManagerEventProcess)(
    void* component,
    unsigned int event);

struct FableRetailFrontendManagerEventTarget
{
    void* component;
    FableRetailFrontendManagerEventCondition condition;
    FableRetailFrontendManagerEventProcess process;
};

inline unsigned int FableRetailFrontendManagerProcessEvent(
    const FableRetailFrontendManagerEventTarget* current,
    const FableRetailFrontendManagerEventTarget* registeredComponents,
    unsigned int registeredComponentCount,
    unsigned int event)
{
    unsigned int processedCount = 0;
    if (current != 0 && current->component != 0)
    {
        if (
            current->condition != 0 &&
            current->process != 0 &&
            current->condition(current->component, event))
        {
            current->process(current->component, event);
            return 1;
        }
        return 0;
    }

    for (unsigned int index = 0; index != registeredComponentCount; ++index)
    {
        const FableRetailFrontendManagerEventTarget& target =
            registeredComponents[index];
        if (
            target.component != 0 &&
            target.condition != 0 &&
            target.process != 0 &&
            target.condition(target.component, event))
        {
            target.process(target.component, event);
            ++processedCount;
        }
    }
    return processedCount;
}

inline bool FableRetailFrontendListProcessEvent(
    unsigned int event,
    const FableRetailFrontendListProcessContext& context,
    unsigned int* dispatchedEvent)
{
    if (dispatchedEvent == 0)
        return false;
    if (!context.viewportConditionPasses ||
        !context.hoverConditionPasses ||
        !context.managerActionConditionPasses ||
        !context.componentConditionPasses)
    {
        return false;
    }
    if (event != FableRetailFrontendListEventUp &&
        event != FableRetailFrontendListEventDown)
    {
        return false;
    }
    *dispatchedEvent = event;
    return true;
}

static const float kFableRetailFrontendWheelThreshold = 0.0001f;

inline bool FableRetailFrontendWheelAction(
    float movement,
    unsigned int* action)
{
    if (action == 0)
        return false;
    if (movement > kFableRetailFrontendWheelThreshold)
    {
        *action = FableRetailFrontendActionWheelUp;
        return true;
    }
    if (movement < -kFableRetailFrontendWheelThreshold)
    {
        *action = FableRetailFrontendActionWheelDown;
        return true;
    }
    return false;
}

inline bool FableRetailFrontendListEventFromAction(
    unsigned int action,
    unsigned int* event)
{
    if (event == 0)
        return false;
    if (action == FableRetailFrontendActionWheelUp)
    {
        *event = FableRetailFrontendListEventUp;
        return true;
    }
    if (action == FableRetailFrontendActionWheelDown)
    {
        *event = FableRetailFrontendListEventDown;
        return true;
    }
    return false;
}

// CClickable::ProcessEvent keeps the left-button press state on the
// component. Event 0x1A is raised only for an active component; event 0x1C
// only consumes a previously pressed component. The owner callback is the
// virtual call through `this - 4` at slots +0x248/+0x24c; the hover callback
// is the base CHoverable::ProcessEvent call made by both paths.
typedef void (*FableRetailFrontendClickableOwnerCallback)(void* owner);
typedef void (*FableRetailFrontendClickableHoverCallback)(
    void* component,
    unsigned int event);

inline bool FableRetailFrontendClickableProcessLeftEvent(
    unsigned int event,
    bool active,
    float currentTime,
    bool* pressed,
    float* lastEventTime,
    void* owner,
    FableRetailFrontendClickableOwnerCallback ownerPressed,
    FableRetailFrontendClickableOwnerCallback ownerReleased,
    void* component,
    FableRetailFrontendClickableHoverCallback hoverProcess)
{
    if (pressed == 0)
        return false;
    if (event == FableRetailFrontendClickableLeftClicked)
    {
        if (active)
        {
            if (ownerPressed != 0)
                ownerPressed(owner);
            *pressed = true;
        }
        if (lastEventTime != 0)
            *lastEventTime = currentTime;
        if (hoverProcess != 0)
            hoverProcess(component, event);
        return active;
    }
    if (event == FableRetailFrontendClickableLeftUnclicked)
    {
        const bool wasPressed = *pressed;
        if (wasPressed)
        {
            if (ownerReleased != 0)
                ownerReleased(owner);
            *pressed = false;
        }
        if (lastEventTime != 0)
            *lastEventTime = currentTime;
        if (hoverProcess != 0)
            hoverProcess(component, event);
        return wasPressed;
    }
    return false;
}

// Compatibility wrapper for focused callers that only need the recovered
// press/release state machine and have no owner object to invoke yet.
inline bool FableRetailFrontendClickableProcessEvent(
    unsigned int event,
    bool active,
    bool* pressed)
{
    return FableRetailFrontendClickableProcessLeftEvent(
        event,
        active,
        0.0f,
        pressed,
        0,
        0,
        0,
        0,
        0,
        0);
}

/*
 * The retail body also handles held/released click events 0x1B/0x1D and
 * timed activation events 0x1F/0x20. They are intentionally not collapsed
 * into this left-click helper: the visual checkpoint has not recovered a
 * live CClickable owner/time source for those event classes.
 */
#if 0
        *pressed = true;
        return true;
    }
    if (event == FableRetailFrontendClickableLeftUnclicked)
    {
        if (!*pressed)
            return false;
        *pressed = false;
        return true;
    }
    return false;
}
#endif

// CFrontendGameComponent::ProcessTextInputCharacter @ 0x004944E0.
// Retail stores the low byte of each unsigned-short character, reserves the
// final byte for a terminator, and treats character 8 as backspace.
inline bool FableRetailFrontendProcessTextInputCharacter(
    unsigned short character,
    char* text,
    unsigned int* length)
{
    if (text == 0 || length == 0)
        return false;
    if (character == 8)
    {
        if (*length == 0)
            return false;
        --*length;
        text[*length] = '\0';
        return true;
    }
    if (*length >= 127)
        return false;
    text[*length] = static_cast<char>(character);
    ++*length;
    text[*length] = '\0';
    return true;
}

#endif
