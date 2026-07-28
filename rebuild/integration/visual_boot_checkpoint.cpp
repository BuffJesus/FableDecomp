#include "fable_visual_boot.h"
#include "fable_visual_d3d9.h"
#include "frontend_startup_sequence.h"
#include "retail_video_bridge.h"

#include <string.h>

// The recovered VC7.1 compiler bundle does not yet contain a Platform SDK.
// Keep this checkpoint buildable with that compiler by declaring only the
// stable Win32 surface used here. These declarations can disappear when the
// period-correct SDK is added.
typedef int FableBool;
typedef unsigned short FableAtom;
typedef unsigned short FableWord;
typedef unsigned long FableDword;
typedef long FableLong;
typedef unsigned int FableUint;
typedef unsigned long FableWordParameter;
typedef long FableLongParameter;
typedef long FableResult;
typedef void* FableWindow;
typedef void* FableDeviceContext;
typedef void* FableBitmap;
typedef void* FableBrush;
typedef void* FableGraphicsObject;
typedef void* FableCursor;
typedef void* FableIcon;
typedef void* FableMenu;

struct FablePoint
{
    FableLong x;
    FableLong y;
};

struct FableRectangle
{
    FableLong left;
    FableLong top;
    FableLong right;
    FableLong bottom;
};

struct FableMessage
{
    FableWindow window;
    FableUint message;
    FableWordParameter wordParameter;
    FableLongParameter longParameter;
    FableDword time;
    FablePoint point;
};

struct FablePaint
{
    FableDeviceContext deviceContext;
    FableBool erase;
    FableRectangle paint;
    FableBool restore;
    FableBool incrementalUpdate;
    unsigned char reserved[32];
};

struct FableBitmapInfo
{
    FableLong type;
    FableLong width;
    FableLong height;
    FableLong widthBytes;
    FableWord planes;
    FableWord bitsPerPixel;
    void* pixels;
};

typedef FableResult (FABLE_STDCALL *FableWindowProcedure)(
    FableWindow,
    FableUint,
    FableWordParameter,
    FableLongParameter);

struct FableWindowClass
{
    FableUint size;
    FableUint style;
    FableWindowProcedure procedure;
    int classExtraBytes;
    int windowExtraBytes;
    FableInstanceHandle instance;
    FableIcon icon;
    FableCursor cursor;
    FableBrush background;
    const char* menuName;
    const char* className;
    FableIcon smallIcon;
};

extern "C"
{
    __declspec(dllimport) FableDeviceContext FABLE_STDCALL BeginPaint(
        FableWindow window,
        FablePaint* paint);
    __declspec(dllimport) FableBool FABLE_STDCALL EndPaint(
        FableWindow window,
        const FablePaint* paint);
    __declspec(dllimport) int FABLE_STDCALL FillRect(
        FableDeviceContext context,
        const FableRectangle* rectangle,
        FableBrush brush);
    __declspec(dllimport) FableBool FABLE_STDCALL GetClientRect(
        FableWindow window,
        FableRectangle* rectangle);
    __declspec(dllimport) void* FABLE_STDCALL LoadImageA(
        FableInstanceHandle instance,
        const char* name,
        FableUint type,
        int desiredWidth,
        int desiredHeight,
        FableUint loadFlags);
    __declspec(dllimport) FableCursor FABLE_STDCALL LoadCursorA(
        FableInstanceHandle instance,
        const char* name);
    __declspec(dllimport) FableAtom FABLE_STDCALL RegisterClassExA(
        const FableWindowClass* windowClass);
    __declspec(dllimport) FableBool FABLE_STDCALL UnregisterClassA(
        const char* className,
        FableInstanceHandle instance);
    __declspec(dllimport) FableBool FABLE_STDCALL AdjustWindowRectEx(
        FableRectangle* rectangle,
        FableDword style,
        FableBool hasMenu,
        FableDword extendedStyle);
    __declspec(dllimport) FableWindow FABLE_STDCALL CreateWindowExA(
        FableDword extendedStyle,
        const char* className,
        const char* title,
        FableDword style,
        int x,
        int y,
        int width,
        int height,
        FableWindow parent,
        FableMenu menu,
        FableInstanceHandle instance,
        void* parameter);
    __declspec(dllimport) FableBool FABLE_STDCALL ShowWindow(
        FableWindow window,
        int showCommand);
    __declspec(dllimport) FableBool FABLE_STDCALL UpdateWindow(
        FableWindow window);
    __declspec(dllimport) FableBool FABLE_STDCALL InvalidateRect(
        FableWindow window,
        const FableRectangle* rectangle,
        FableBool erase);
    __declspec(dllimport) FableBool FABLE_STDCALL SetWindowTextA(
        FableWindow window,
        const char* text);
    __declspec(dllimport) FableWordParameter FABLE_STDCALL SetTimer(
        FableWindow window,
        FableWordParameter identifier,
        FableUint intervalMilliseconds,
        void* timerProcedure);
    __declspec(dllimport) FableBool FABLE_STDCALL KillTimer(
        FableWindow window,
        FableWordParameter identifier);
    __declspec(dllimport) FableResult FABLE_STDCALL DefWindowProcA(
        FableWindow window,
        FableUint message,
        FableWordParameter wordParameter,
        FableLongParameter longParameter);
    __declspec(dllimport) void FABLE_STDCALL PostQuitMessage(int exitCode);
    __declspec(dllimport) FableBool FABLE_STDCALL GetMessageA(
        FableMessage* message,
        FableWindow window,
        FableUint minimumMessage,
        FableUint maximumMessage);
    __declspec(dllimport) FableBool FABLE_STDCALL TranslateMessage(
        const FableMessage* message);
    __declspec(dllimport) FableResult FABLE_STDCALL DispatchMessageA(
        const FableMessage* message);

    __declspec(dllimport) FableGraphicsObject FABLE_STDCALL GetStockObject(
        int object);
    __declspec(dllimport) int FABLE_STDCALL GetObjectA(
        FableGraphicsObject object,
        int size,
        void* destination);
    __declspec(dllimport) FableDeviceContext FABLE_STDCALL CreateCompatibleDC(
        FableDeviceContext context);
    __declspec(dllimport) FableGraphicsObject FABLE_STDCALL SelectObject(
        FableDeviceContext context,
        FableGraphicsObject object);
    __declspec(dllimport) int FABLE_STDCALL SetStretchBltMode(
        FableDeviceContext context,
        int mode);
    __declspec(dllimport) FableBool FABLE_STDCALL SetBrushOrgEx(
        FableDeviceContext context,
        int x,
        int y,
        FablePoint* previous);
    __declspec(dllimport) FableBool FABLE_STDCALL StretchBlt(
        FableDeviceContext destination,
        int destinationX,
        int destinationY,
        int destinationWidth,
        int destinationHeight,
        FableDeviceContext source,
        int sourceX,
        int sourceY,
        int sourceWidth,
        int sourceHeight,
        FableDword operation);
    __declspec(dllimport) FableBool FABLE_STDCALL DeleteDC(
        FableDeviceContext context);
    __declspec(dllimport) FableBool FABLE_STDCALL DeleteObject(
        FableGraphicsObject object);
}

void FABLE_FASTCALL FableResolveUiComponentTransform(
    const FableUiVector2* localPosition,
    const FableUiVector2* localScale,
    const FableUiResolvedTransform* parent,
    FableUiResolvedTransform* resolved)
{
    resolved->position.x = localPosition->x + parent->position.x;
    resolved->position.y = localPosition->y + parent->position.y;
    resolved->truePosition.x =
        localPosition->x + parent->truePosition.x;
    resolved->truePosition.y =
        localPosition->y + parent->truePosition.y;
    resolved->scale.x = localScale->x * parent->scale.x;
    resolved->scale.y = localScale->y * parent->scale.y;
    resolved->trueScale.x = localScale->x * parent->trueScale.x;
    resolved->trueScale.y = localScale->y * parent->trueScale.y;
}

void FABLE_FASTCALL FableResolveUiGeneratedChildOrigins(
    const FableUiVector2* childLocalPosition,
    const FableUiVector2* uiScale,
    const FableUiResolvedTransform* table,
    FableUiVector2* ordinaryPosition,
    FableUiVector2* truePosition)
{
    ordinaryPosition->x =
        childLocalPosition->x + table->position.x;
    ordinaryPosition->y =
        childLocalPosition->y + table->position.y;
    truePosition->x =
        childLocalPosition->x * uiScale->x +
        table->truePosition.x;
    truePosition->y =
        childLocalPosition->y * uiScale->y +
        table->truePosition.y;
}

void FABLE_FASTCALL FablePlanUiTableLine(
    bool vertical,
    const FableUiVector2* start,
    const FableUiVector2* primarySize,
    const FableUiVector2* secondarySize,
    unsigned long repetitions,
    unsigned long resourceKeyOffset,
    const fable_i32* resourceKeys,
    unsigned int resourceKeyCount,
    FableUiTableLineSecondary* secondaryOutput,
    unsigned int secondaryCapacity,
    FableUiTableLinePlan* plan)
{
    plan->primaryPosition = *start;
    plan->primaryZoom.x = vertical ? 1.0f :
        static_cast<float>(repetitions);
    plan->primaryZoom.y = vertical ?
        static_cast<float>(repetitions) : 1.0f;

    FableUiVector2 cursor = *start;
    if (vertical)
        cursor.y += primarySize->y;
    else
        cursor.x += primarySize->x;

    unsigned int emitted = 0;
    for (
        unsigned long iteration = 0;
        iteration != repetitions;
        ++iteration)
    {
        const fable_i32 resourceKey = static_cast<fable_i32>(
            iteration - resourceKeyOffset);
        bool present = false;
        for (
            unsigned int keyIndex = 0;
            keyIndex != resourceKeyCount;
            ++keyIndex)
        {
            if (resourceKeys[keyIndex] == resourceKey)
            {
                present = true;
                break;
            }
        }
        if (!present)
            continue;

        if (
            secondaryOutput != 0 &&
            emitted < secondaryCapacity)
        {
            secondaryOutput[emitted].position = cursor;
            secondaryOutput[emitted].resourceKey = resourceKey;
        }
        ++emitted;
        if (vertical)
            cursor.y += secondarySize->y;
        else
            cursor.x += secondarySize->x;
    }
    plan->finalCursor = cursor;
    plan->secondaryCount = emitted;
    plan->generatedChildCount = 1 + emitted;
}

namespace
{
    bool HasUiTableSpriteMask(
        unsigned int availableSpriteMask,
        unsigned int spriteKey)
    {
        return (
            availableSpriteMask &
            (1UL << spriteKey)) != 0;
    }

    bool HasUiTableSprite(
        const FableUiTableSpritePlanInput* input,
        unsigned int spriteKey)
    {
        return HasUiTableSpriteMask(
            input->availableSpriteMask,
            spriteKey);
    }

    void AppendUiTableSpritePlanEvent(
        FableUiTableSpritePlanEvent* eventOutput,
        unsigned int eventOutputCapacity,
        FableUiTableSpritePlan* plan,
        unsigned int kind,
        unsigned int primarySpriteKey,
        fable_i32 secondarySpriteKey,
        unsigned int separatorIndex,
        unsigned int repeatCount,
        fable_i32 resourceKeyOffset)
    {
        if (
            eventOutput != 0 &&
            plan->writtenEventCount < eventOutputCapacity)
        {
            FableUiTableSpritePlanEvent& event =
                eventOutput[plan->writtenEventCount];
            event.kind = kind;
            event.primarySpriteKey = primarySpriteKey;
            event.secondarySpriteKey = secondarySpriteKey;
            event.separatorIndex = separatorIndex;
            event.repeatCount = repeatCount;
            event.resourceKeyOffset = resourceKeyOffset;
            ++plan->writtenEventCount;
        }
        ++plan->logicalEventCount;
    }
}

void FABLE_FASTCALL FablePlanUiTableSpriteComposition(
    const FableUiTableSpritePlanInput* input,
    FableUiTableSpritePlanEvent* eventOutput,
    unsigned int eventOutputCapacity,
    FableUiTableSpritePlan* plan)
{
    plan->logicalEventCount = 0;
    plan->writtenEventCount = 0;

    if (HasUiTableSprite(input, 0))
    {
        AppendUiTableSpritePlanEvent(
            eventOutput,
            eventOutputCapacity,
            plan,
            FableUiTableSpriteCorner,
            0,
            -1,
            0,
            0,
            0);
    }
    if (
        input->topRightCornerVisible != 0 &&
        HasUiTableSprite(input, 1))
    {
        AppendUiTableSpritePlanEvent(
            eventOutput,
            eventOutputCapacity,
            plan,
            FableUiTableSpriteCorner,
            1,
            -1,
            0,
            0,
            0);
    }
    if (
        input->bottomLeftCornerVisible != 0 &&
        HasUiTableSprite(input, 2))
    {
        AppendUiTableSpritePlanEvent(
            eventOutput,
            eventOutputCapacity,
            plan,
            FableUiTableSpriteCorner,
            2,
            -1,
            0,
            0,
            0);
    }
    if (
        input->bottomRightCornerVisible != 0 &&
        HasUiTableSprite(input, 3))
    {
        AppendUiTableSpritePlanEvent(
            eventOutput,
            eventOutputCapacity,
            plan,
            FableUiTableSpriteCorner,
            3,
            -1,
            0,
            0,
            0);
    }

    if (HasUiTableSprite(input, 4))
    {
        AppendUiTableSpritePlanEvent(
            eventOutput,
            eventOutputCapacity,
            plan,
            FableUiTableSpriteHorizontalLine,
            4,
            HasUiTableSprite(input, 9) ? 9 : -1,
            0,
            input->horizontalRepeatCount,
            0);
        for (
            unsigned int separator = 0;
            separator != input->horizontalSeparatorCount;
            ++separator)
        {
            AppendUiTableSpritePlanEvent(
                eventOutput,
                eventOutputCapacity,
                plan,
                FableUiTableSpriteHorizontalLine,
                4,
                HasUiTableSprite(input, 12) ? 12 : -1,
                separator,
                input->horizontalRepeatCount,
                input->horizontalInteriorResourceKeyOffset);
        }
    }
    if (HasUiTableSprite(input, 5))
    {
        AppendUiTableSpritePlanEvent(
            eventOutput,
            eventOutputCapacity,
            plan,
            FableUiTableSpriteHorizontalLine,
            5,
            HasUiTableSprite(input, 8) ? 8 : -1,
            0,
            input->horizontalRepeatCount,
            0);
    }
    if (HasUiTableSprite(input, 6))
    {
        AppendUiTableSpritePlanEvent(
            eventOutput,
            eventOutputCapacity,
            plan,
            FableUiTableSpriteVerticalLine,
            6,
            HasUiTableSprite(input, 10) ? 10 : -1,
            0,
            input->verticalRepeatCount,
            0);
        for (
            unsigned int separator = 0;
            separator != input->verticalSeparatorCount;
            ++separator)
        {
            AppendUiTableSpritePlanEvent(
                eventOutput,
                eventOutputCapacity,
                plan,
                FableUiTableSpriteVerticalLine,
                6,
                HasUiTableSprite(input, 12) ? 12 : -1,
                separator,
                input->verticalRepeatCount,
                input->verticalInteriorResourceKeyOffset);
        }
    }
    if (HasUiTableSprite(input, 7))
    {
        AppendUiTableSpritePlanEvent(
            eventOutput,
            eventOutputCapacity,
            plan,
            FableUiTableSpriteVerticalLine,
            7,
            HasUiTableSprite(input, 11) ? 11 : -1,
            0,
            input->verticalRepeatCount,
            0);
    }
}

void FABLE_FASTCALL FablePlanUiTableGeometry(
    const FableUiTableGeometryInput* input,
    FableUiTableInteriorLine* horizontalInteriorOutput,
    unsigned int horizontalInteriorCapacity,
    FableUiTableInteriorLine* verticalInteriorOutput,
    unsigned int verticalInteriorCapacity,
    FableUiTableGeometryPlan* plan)
{
    const float cornerVisibilityEpsilonSquared = 9.999999e-09f;
    const float horizontalExtent =
        input->horizontalEdgeSize.x *
        static_cast<float>(input->horizontalRepeatCount);
    const float verticalExtent =
        input->verticalEdgeSize.y *
        static_cast<float>(input->verticalRepeatCount);

    FableUiVector2 topRight = {horizontalExtent, 0.0f};
    FableUiVector2 bottomLeft = {0.0f, verticalExtent};
    FableUiVector2 bottomRight = {
        horizontalExtent,
        verticalExtent};
    FableUiVector2 topLeft = {0.0f, 0.0f};

    plan->emittedCornerMask = 0;
    plan->horizontalExtent = horizontalExtent;
    plan->verticalExtent = verticalExtent;
    plan->logicalHorizontalInteriorCount =
        input->horizontalSeparatorCount;
    plan->writtenHorizontalInteriorCount = 0;
    plan->logicalVerticalInteriorCount =
        input->verticalSeparatorCount;
    plan->writtenVerticalInteriorCount = 0;

    for (unsigned int corner = 0; corner != 4; ++corner)
    {
        plan->cornerPosition[corner].x = 0.0f;
        plan->cornerPosition[corner].y = 0.0f;
    }

    if (HasUiTableSpriteMask(input->availableSpriteMask, 0))
    {
        plan->cornerPosition[0] = topLeft;
        plan->emittedCornerMask |= 1UL << 0;
        topLeft.x += input->cornerSize[0].x;
        bottomLeft.y += input->cornerSize[0].y;
        topRight.x += input->cornerSize[0].x;
    }

    if (
        HasUiTableSpriteMask(input->availableSpriteMask, 1) &&
        topRight.x * topRight.x + topRight.y * topRight.y >
            cornerVisibilityEpsilonSquared)
    {
        plan->cornerPosition[1] = topRight;
        plan->emittedCornerMask |= 1UL << 1;
        topRight.x +=
            input->cornerSize[1].x -
            input->horizontalEdgeSize.x;
        topRight.y += input->cornerSize[1].y;
        bottomRight.y += input->cornerSize[1].y;
    }

    if (
        HasUiTableSpriteMask(input->availableSpriteMask, 2) &&
        bottomLeft.x * bottomLeft.x + bottomLeft.y * bottomLeft.y >
            cornerVisibilityEpsilonSquared)
    {
        plan->cornerPosition[2] = bottomLeft;
        plan->emittedCornerMask |= 1UL << 2;
        bottomLeft.y -= verticalExtent;
        bottomRight.x += input->cornerSize[2].x;
    }

    if (
        HasUiTableSpriteMask(input->availableSpriteMask, 3) &&
        bottomRight.x * bottomRight.x +
                bottomRight.y * bottomRight.y >
            cornerVisibilityEpsilonSquared)
    {
        plan->cornerPosition[3] = bottomRight;
        plan->emittedCornerMask |= 1UL << 3;
        bottomRight.x -= horizontalExtent;
        bottomRight.y +=
            input->cornerSize[3].y -
            input->verticalEdgeSize.y;
    }

    plan->topLineStart = topLeft;
    plan->bottomLineStart = bottomRight;
    plan->leftLineStart = bottomLeft;
    plan->rightLineStart = topRight;

    fable_i32 horizontalResourceKeyOffset = -1;
    if (
        (plan->emittedCornerMask & (1UL << 0)) != 0 &&
        input->horizontalEdgeSize.x != 0.0f)
    {
        horizontalResourceKeyOffset =
            static_cast<fable_i32>(
                input->cornerSize[0].x /
                input->horizontalEdgeSize.x) -
            1;
    }

    for (
        unsigned int separator = 0;
        separator != input->horizontalSeparatorCount;
        ++separator)
    {
        FableUiTableInteriorLine line = {};
        line.position.x =
            bottomLeft.x + input->horizontalEdgeSize.x;
        line.position.y =
            bottomLeft.y +
            static_cast<float>(
                input->horizontalSeparatorTiles[separator]) *
                input->verticalEdgeSize.y;
        if (input->horizontalEdgeSize.x != 0.0f)
        {
            line.repeatCount = static_cast<unsigned long>(
                (topRight.x - line.position.x) /
                input->horizontalEdgeSize.x);
        }
        line.resourceKeyOffset = horizontalResourceKeyOffset;

        if (
            horizontalInteriorOutput != 0 &&
            plan->writtenHorizontalInteriorCount <
                horizontalInteriorCapacity)
        {
            horizontalInteriorOutput[
                plan->writtenHorizontalInteriorCount] = line;
            ++plan->writtenHorizontalInteriorCount;
        }
    }

    fable_i32 verticalResourceKeyOffset = -1;
    if (
        (plan->emittedCornerMask & (1UL << 0)) != 0 &&
        input->verticalEdgeSize.y != 0.0f)
    {
        verticalResourceKeyOffset =
            static_cast<fable_i32>(
                input->cornerSize[0].y /
                input->verticalEdgeSize.y) -
            1;
    }

    for (
        unsigned int separator = 0;
        separator != input->verticalSeparatorCount;
        ++separator)
    {
        FableUiTableInteriorLine line = {};
        line.position.x =
            topLeft.x +
            static_cast<float>(
                input->verticalSeparatorTiles[separator]) *
                input->horizontalEdgeSize.x;
        line.position.y =
            topLeft.y + input->verticalEdgeSize.y;
        if (input->verticalEdgeSize.y != 0.0f)
        {
            line.repeatCount = static_cast<unsigned long>(
                (bottomRight.y - line.position.y) /
                input->verticalEdgeSize.y);
        }
        line.resourceKeyOffset = verticalResourceKeyOffset;

        if (
            verticalInteriorOutput != 0 &&
            plan->writtenVerticalInteriorCount <
                verticalInteriorCapacity)
        {
            verticalInteriorOutput[
                plan->writtenVerticalInteriorCount] = line;
            ++plan->writtenVerticalInteriorCount;
        }
    }
}

void FABLE_FASTCALL FablePlanUiListSelection(
    long requestedChild,
    const unsigned char* visibleChildren,
    unsigned int childCount,
    FableUiListSelectionPlan* plan)
{
    plan->selectedChild = requestedChild;
    plan->firstVisibleChild = -1;
    plan->lastVisibleChild = -1;
    plan->scrollDelta.x = 0.0f;
    plan->scrollDelta.y = 0.0f;
    plan->requestsScroll = false;

    if (
        requestedChild < 0 ||
        static_cast<unsigned long>(requestedChild) >= childCount ||
        visibleChildren[requestedChild] != 0)
    {
        return;
    }

    unsigned int firstVisible = 0xffffffffUL;
    long lastVisible = -1;
    for (unsigned int child = 0; child != childCount; ++child)
    {
        if (
            firstVisible == 0xffffffffUL &&
            visibleChildren[child] != 0)
        {
            firstVisible = child;
        }
        if (
            firstVisible != 0xffffffffUL &&
            visibleChildren[child] == 0)
        {
            lastVisible = static_cast<long>(child) - 1;
            break;
        }
    }
    if (lastVisible == -1)
        lastVisible = static_cast<long>(childCount) - 1;

    plan->firstVisibleChild =
        firstVisible == 0xffffffffUL
            ? -1
            : static_cast<long>(firstVisible);
    plan->lastVisibleChild = lastVisible;

    if (lastVisible < requestedChild)
    {
        plan->scrollDelta.y =
            static_cast<float>(requestedChild - lastVisible) * -25.0f;
        plan->requestsScroll = true;
    }
    else if (firstVisible != 0xffffffffUL)
    {
        plan->scrollDelta.y =
            static_cast<float>(
                static_cast<long>(firstVisible) - requestedChild) *
            25.0f;
        plan->requestsScroll = true;
    }
}

void FABLE_FASTCALL FablePlanUiFrontEndListInitialOffsets(
    unsigned long childCount,
    FableUiFrontEndListOffsetPlan* plan)
{
    (void)childCount;
    plan->interpolationScalar = 1.0f;
    plan->positionOffset.x = 0.0f;
    plan->positionOffset.y = 0.0f;
}

void FABLE_FASTCALL FablePlanUiListInitialOffsets(
    unsigned long childCount,
    const FableUiVector2* definitionPositionOffset,
    unsigned char alphaFalloff,
    bool zeroInitialPosition,
    FableUiFrontEndListOffsetPlan* plan)
{
    if (definitionPositionOffset == 0 || plan == 0)
        return;

    const unsigned long centreOffset =
        (childCount & 1) != 0
            ? childCount >> 1
            : (childCount >> 1) - 1;
    const float centreOffsetFloat =
        static_cast<float>(centreOffset);
    const float alphaStep =
        static_cast<float>(alphaFalloff) *
        0.003921568859368563f;

    plan->interpolationScalar =
        1.0f - alphaStep * centreOffsetFloat;
    if (zeroInitialPosition)
    {
        plan->positionOffset.x = 0.0f;
        plan->positionOffset.y = 0.0f;
    }
    else
    {
        plan->positionOffset.x =
            centreOffsetFloat * definitionPositionOffset->x;
        plan->positionOffset.y =
            -(centreOffsetFloat * definitionPositionOffset->y);
    }
}

void FABLE_FASTCALL FablePlanUiListRecomputeOffsets(
    const FableUiListRecomputeInput* input,
    FableUiListRecomputedChild* childOutput,
    unsigned int childOutputCapacity,
    FableUiListRecomputePlan* plan)
{
    if (input == 0 || plan == 0)
        return;

    plan->logicalChildCount = input->childCount;
    plan->writtenChildCount = 0;
    plan->positionStateMask =
        (1u << 0) | (1u << 1) | (1u << 4) |
        (1u << 5) | (1u << 6);
    plan->alphaStateMask =
        (1u << 1) | (1u << 4) | (1u << 5);

    FableUiVector2 position = input->initialPosition;
    FableUiVector2 positionStep;
    positionStep.x = -input->definitionPositionOffset.x;
    positionStep.y = input->definitionPositionOffset.y;
    const float alphaStep =
        static_cast<float>(input->alphaFalloff) *
        0.003921568859368563f;
    float alphaStepToUse = alphaStep;
    float alpha = input->initialAlpha;

    for (unsigned int child = 0; child != input->childCount; ++child)
    {
        unsigned long byteAlpha = 0;
        if (alpha > 1.0f)
        {
            byteAlpha = 255;
        }
        else if (alpha >= 0.0f)
        {
            byteAlpha =
                static_cast<unsigned long>(alpha * 255.0f);
        }

        if (
            childOutput != 0 &&
            plan->writtenChildCount < childOutputCapacity)
        {
            childOutput[plan->writtenChildCount].position = position;
            childOutput[plan->writtenChildCount].alpha =
                static_cast<unsigned char>(byteAlpha);
            ++plan->writtenChildCount;
        }

        position.y += positionStep.y;
        const float nextAlpha = alpha + alphaStepToUse;
        if (nextAlpha >= 0.999f)
        {
            alphaStepToUse = -alphaStepToUse;
            positionStep.x = -positionStep.x;
            if (alpha >= 1.0f)
            {
                position.x += positionStep.x;
                alpha += alphaStepToUse;
            }
            else
            {
                position.x = 0.0f;
                alpha = 1.0f;
            }
        }
        else
        {
            alpha = nextAlpha;
            position.x += positionStep.x;
        }
    }

    plan->finalPosition = position;
    plan->finalAlpha = alpha;
}

void FABLE_FASTCALL FablePlanUiFrontEndListScroll(
    bool scrollDown,
    unsigned int childCount,
    long selectedChild,
    bool stopsAtEnds,
    unsigned char alphaFalloff,
    unsigned char* childAlphaOutput,
    unsigned int childAlphaCapacity,
    FableUiFrontEndListScrollPlan* plan)
{
    if (plan == 0)
        return;

    plan->previousSelectedChild = selectedChild;
    plan->selectedChild = selectedChild;
    plan->rowTranslation.x = 0.0f;
    plan->rowTranslation.y = 0.0f;
    plan->previousChildState = 0;
    plan->selectedChildState = 0;
    plan->logicalAlphaCount = 0;
    plan->writtenAlphaCount = 0;
    plan->moved = false;
    plan->blockedAtBoundary = false;
    plan->requestsInvalidAction = false;
    plan->requestsMoveAction = false;

    if (childCount <= 1)
    {
        plan->requestsInvalidAction = true;
        return;
    }

    const long lastChild = static_cast<long>(childCount - 1);
    const bool atBoundary =
        scrollDown ? selectedChild == lastChild : selectedChild == 0;
    if (atBoundary && stopsAtEnds)
    {
        plan->blockedAtBoundary = true;
        plan->requestsInvalidAction = true;
        return;
    }

    long nextChild = selectedChild + (scrollDown ? 1 : -1);
    if (nextChild < 0)
        nextChild = lastChild;
    else if (nextChild > lastChild)
        nextChild = 0;

    plan->selectedChild = nextChild;
    plan->rowTranslation.y = scrollDown ? -30.0f : 30.0f;
    plan->previousChildState = 4;
    plan->selectedChildState = 3;
    plan->moved = true;
    plan->requestsMoveAction = true;

    if (!stopsAtEnds)
        return;

    plan->logicalAlphaCount = childCount;
    for (unsigned int child = 0; child != childCount; ++child)
    {
        const long signedDistance =
            nextChild - static_cast<long>(child);
        const unsigned long distance =
            static_cast<unsigned long>(
                signedDistance < 0 ? -signedDistance : signedDistance);
        float alpha =
            1.0f -
            static_cast<float>(distance) *
            static_cast<float>(alphaFalloff) *
            0.003921568859368563f;
        if (alpha < 0.0f)
            alpha = 0.0f;
        unsigned long byteAlpha =
            static_cast<unsigned long>(alpha * 255.0f);
        if (byteAlpha > 255)
            byteAlpha = 255;
        if (
            childAlphaOutput != 0 &&
            plan->writtenAlphaCount < childAlphaCapacity)
        {
            childAlphaOutput[plan->writtenAlphaCount] =
                static_cast<unsigned char>(byteAlpha);
            ++plan->writtenAlphaCount;
        }
    }
}

namespace
{
    const int kBootArtworkResource = 101;
    const int kBootTitleResource = 102;
    const int kBootPointerResource = 105;
    const int kBootMenuResource = 106;
    const int kBootCoastalResource = 107;
    const int kBootCoastalSunbeamResource = 108;
    const int kBootOptionsResource = 109;
    const int kBootHelpersResource = 110;
    const int kBootBuffJesusMenuResource = 111;
    const char kWindowClassName[] = "FableDecompVisualBootCheckpoint";
    const char kWindowTitle[] = "FableDecomp - Visual Boot Checkpoint";
    const char kProgressReadyWindowTitle[] =
        "FableDecomp - Retail Progress Display Ready";
    const char kProgressActiveWindowTitle[] =
        "FableDecomp - Retail Progress Display Active";
    const char kRetailVideoStartingWindowTitle[] =
        "FableDecomp - Retail WMV Starting";
    const char kRetailVideoPlayingWindowTitle[] =
        "FableDecomp - Retail WMV Playing - Recovered Boot + D3D9";
    const char kRetailVideoUpscaledPlayingWindowTitle[] =
        "FableDecomp - Retail WMV Playing - AI 2x Enhanced";
    const char kRetailVideoSequenceCompleteWindowTitle[] =
        "FableDecomp - Retail Boot Movies Complete - Frontend Checkpoint Ready - Post-Movie Startup Ordered";
    const char kRetailVideoSequenceFailedWindowTitle[] =
        "FableDecomp - Retail Boot Movies Complete - Frontend Startup Boundary Failed";
    const char kRetailVideoCompleteWindowTitle[] =
        "FableDecomp - Retail Movie Complete - Frontend Ready";
#if defined(FABLETLC_RETAIL_FRONTEND_ARTWORK)
    const char kRetailAssetReadyWindowTitle[] =
        "FableDecomp - Retail Frontend Asset + Progress Display Ready";
    const char kRetailAssetActiveWindowTitle[] =
        "FableDecomp - Retail Frontend Asset + Progress Display Active";
    const char kD3DRetailAssetReadyWindowTitle[] =
        "FableDecomp - D3D9 Retail Frontend + Progress Display Ready";
    const char kD3DRetailAssetActiveWindowTitle[] =
        "FableDecomp - D3D9 Retail Frontend + Progress Display Active";
    const char kD3DRetailAssetPresentedReadyWindowTitle[] =
        "FableDecomp - D3D9 Presented via Render2D - Retail Frontend + Progress Display Ready";
    const char kD3DRetailAssetPresentedActiveWindowTitle[] =
        "FableDecomp - D3D9 Presented via Render2D - Retail Frontend + Progress Display Active";
    const char kD3DBuffJesusPresentedWindowTitle[] =
        "FableDecomp - D3D9 Presented via Render2D - BuffJesus Text Variant";
#else
    const char kD3DCheckpointReadyWindowTitle[] =
        "FableDecomp - D3D9 Visual Checkpoint Ready";
    const char kD3DCheckpointActiveWindowTitle[] =
        "FableDecomp - D3D9 Visual Checkpoint Active";
    const char kD3DCheckpointPresentedReadyWindowTitle[] =
        "FableDecomp - D3D9 Presented via Render2D - Visual Checkpoint Ready";
    const char kD3DCheckpointPresentedActiveWindowTitle[] =
        "FableDecomp - D3D9 Presented via Render2D - Visual Checkpoint Active";
#endif

    const FableUint kImageBitmap = 0;
    const FableUint kLoadCreatedDibSection = 0x00002000;
    const int kBlackBrush = 4;
    const int kArrowCursor = 32512;
    const FableUint kClassRedrawHorizontal = 0x0002;
    const FableUint kClassRedrawVertical = 0x0001;
    const FableUint kMessageDestroy = 0x0002;
    const FableUint kMessageSize = 0x0005;
    const FableUint kMessagePaint = 0x000F;
    const FableUint kMessageEraseBackground = 0x0014;
    const FableUint kMessageKeyDown = 0x0100;
    const FableUint kMessageKeyUp = 0x0101;
    const FableUint kMessageTimer = 0x0113;
    const FableUint kMessageMouseMove = 0x0200;
    const FableUint kMessageLeftButtonUp = 0x0202;
    const FableUint kMessageRightButtonUp = 0x0205;
    const FableUint kMessageMiddleButtonUp = 0x0208;
    const FableWordParameter kRetailVideoTimer = 1;
    const FableWordParameter kFrontendAnimationTimer = 2;
    const FableWordParameter kEscapeKey = 0x1B;
    const char* const kRetailBootMovies[] = {
        "lionhead_logo.wmv",
        "microsoft_logo.wmv",
        "intro_comp.wmv"
    };
    const char* const kRetailBootPlayingWindowTitles[] = {
        "FableDecomp - Retail WMV Playing 1/3 - Lionhead",
        "FableDecomp - Retail WMV Playing 2/3 - Microsoft",
        "FableDecomp - Retail WMV Playing 3/3 - Intro"
    };
    const char* const kRetailBootUpscaledPlayingWindowTitles[] = {
        "FableDecomp - Retail WMV Playing 1/3 - Lionhead - AI 2x",
        "FableDecomp - Retail WMV Playing 2/3 - Microsoft - AI 2x",
        "FableDecomp - Retail WMV Playing 3/3 - Intro - AI 2x"
    };
    const unsigned int kRetailBootMovieCount =
        sizeof(kRetailBootMovies) / sizeof(kRetailBootMovies[0]);
    const FableDword kOverlappedWindow = 0x00CF0000UL;
    const int kUseDefaultPosition = static_cast<int>(0x80000000UL);
    const int kShowNormal = 1;
    const int kHalftoneStretch = 4;
    const FableDword kCopySource = 0x00CC0020UL;

    FableBitmap g_BootArtwork = 0;
    FableBitmapInfo g_BootArtworkInfo = {};
    FableBitmap g_BootTitleArtwork = 0;
    FableBitmapInfo g_BootTitleArtworkInfo = {};
    FableBitmap g_BootForestArtwork = 0;
    FableBitmapInfo g_BootForestArtworkInfo = {};
    FableBitmap g_BootSunbeamArtwork = 0;
    FableBitmapInfo g_BootSunbeamArtworkInfo = {};
    FableBitmap g_BootMenuArtwork = 0;
    FableBitmapInfo g_BootMenuArtworkInfo = {};
    FableBitmap g_BootBuffJesusMenuArtwork = 0;
    FableBitmapInfo g_BootBuffJesusMenuArtworkInfo = {};
    FableBitmap g_BootCoastalArtwork = 0;
    FableBitmapInfo g_BootCoastalArtworkInfo = {};
    FableBitmap g_BootCoastalSunbeamArtwork = 0;
    FableBitmapInfo g_BootCoastalSunbeamArtworkInfo = {};
    FableBitmap g_BootOptionsArtwork = 0;
    FableBitmapInfo g_BootOptionsArtworkInfo = {};
    FableBitmap g_BootHelpersArtwork = 0;
    FableBitmapInfo g_BootHelpersArtworkInfo = {};
    bool g_RetailProgressDisplayPresent = false;
    bool g_RetailProgressDisplayActive = false;
    FableWindow g_RetailVideoWindow = 0;
    FableInstanceHandle g_RetailVideoInstance = 0;
    unsigned int g_RetailBootMovieIndex = 0;
    bool g_RetailBootSequenceActive = false;
    bool g_RetailVideoReachedRunningState = false;
    bool g_RetailVideoEscapePressed = false;
    bool g_VisualFrontendVisible = true;
    bool g_VisualMainMenuActive = false;
    unsigned int g_VisualMainMenuSelection = 0;
    bool g_VisualOptionsMenuActive = false;
    unsigned int g_VisualOptionsSelection = 0;
    bool g_VisualOptionsBackHovered = false;
    unsigned int g_VisualDetailScreen = 0;
    const unsigned int kVisualDetailRowCounts[3] = {10, 3, 10};
    const unsigned int kVisualDetailValueCounts[3][10] = {
        {2, 2, 2, 10, 2, 16, 2, 2, 2, 2},
        {11, 11, 11, 0, 0, 0, 0, 0, 0, 0},
        {3, 4, 3, 4, 4, 4, 18, 2, 4, 3}
    };
    const int kVisualDetailRowY[3][10] = {
        {90, 120, 150, 180, 210, 240, 270, 300, 330, 360},
        {130, 190, 250, 0, 0, 0, 0, 0, 0, 0},
        {90, 120, 150, 180, 210, 240, 270, 300, 330, 360}
    };
    const unsigned int kVisualDetailDefaults[3][10] = {
        {0, 0, 1, 4, 1, 15, 1, 1, 1, 1},
        {6, 8, 9, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 1, 1, 1, 2, 0, 1, 0}
    };
    const unsigned int kVisualRedefineDefaults[9] = {
        31, 27, 9, 12, 1, 2, 3, 3, 3
    };
    const unsigned int kVisualRedefineArrowDefaults[9] = {
        41, 42, 43, 44, 1, 2, 3, 3, 3
    };
    unsigned int g_VisualDetailValues[3][10] = {};
    unsigned int g_VisualDetailEntryValues[3][10] = {};
    unsigned int g_VisualDetailSavedValues[3][10] = {};
    unsigned int g_VisualRedefineHover = 0;
    unsigned int g_VisualRedefineResetHover = 0;
    unsigned int g_VisualRedefineSelection = 0;
    unsigned int g_VisualRedefineKeys[9] = {};
    unsigned int g_VisualRedefineEntryKeys[9] = {};
    unsigned int g_VisualRedefineSavedKeys[9] = {};
    bool g_VisualQuitPromptActive = false;
    unsigned int g_VisualQuitHover = 0;
    unsigned long g_FrontendPostMovieStepMask = 0;

    void ResolveVisualGeneratedChildOrigin(
        float parentX,
        float parentY,
        float tableX,
        float tableY,
        float childX,
        float childY,
        FableUiVector2* origin)
    {
        FableUiResolvedTransform parent = {};
        parent.position.x = parentX;
        parent.position.y = parentY;
        parent.truePosition = parent.position;
        parent.scale.x = 1.0f;
        parent.scale.y = 1.0f;
        parent.trueScale = parent.scale;

        FableUiVector2 tablePosition = {tableX, tableY};
        FableUiVector2 tableScale = {1.0f, 1.0f};
        FableUiResolvedTransform table = {};
        FableResolveUiComponentTransform(
            &tablePosition,
            &tableScale,
            &parent,
            &table);

        FableUiVector2 childPosition = {childX, childY};
        FableUiVector2 uiScale = {1.0f, 1.0f};
        FableUiVector2 trueOrigin = {};
        FableResolveUiGeneratedChildOrigins(
            &childPosition,
            &uiScale,
            &table,
            origin,
            &trueOrigin);
    }

    bool FindVisualMainMenuRow(
        int mouseX,
        int mouseY,
        unsigned int* rowFound)
    {
        static const int rowOffsets[7] = {
            0, 30, 60, 120, 180, 210, 240
        };
        for (unsigned int row = 0; row != 7; ++row)
        {
            // frontend.bin roots the list at (200,200). UI_BUTTON_BIG and
            // UI_BUTTON tables are (-140,-7) and (-80,-7); the horizontal
            // CTable builder places the left generated sprite at local x=60.
            FableUiVector2 origin = {};
            ResolveVisualGeneratedChildOrigin(
                200.0f,
                200.0f,
                row == 0 ? -140.0f : -80.0f,
                -7.0f + static_cast<float>(rowOffsets[row]),
                60.0f,
                0.0f,
                &origin);
            const int rowLeft = static_cast<int>(origin.x);
            const int rowRight = rowLeft + (row == 0 ? 400 : 280);
            const int rowTop = static_cast<int>(origin.y);
            if (
                mouseX >= rowLeft &&
                mouseX < rowRight &&
                mouseY >= rowTop &&
                mouseY < rowTop + 30)
            {
                *rowFound = row;
                return true;
            }
        }
        return false;
    }

    bool FindVisualOptionsMenuRow(
        int mouseX,
        int mouseY,
        unsigned int* rowFound)
    {
        for (unsigned int row = 0; row != 4; ++row)
        {
            FableUiVector2 origin = {};
            ResolveVisualGeneratedChildOrigin(
                200.0f,
                150.0f,
                -80.0f,
                -7.0f + static_cast<float>(row * 30),
                60.0f,
                0.0f,
                &origin);
            const int rowLeft = static_cast<int>(origin.x);
            const int rowTop = static_cast<int>(origin.y);
            if (
                mouseX >= rowLeft &&
                mouseX < rowLeft + 280 &&
                mouseY >= rowTop &&
                mouseY < rowTop + 30)
            {
                *rowFound = row;
                return true;
            }
        }
        return false;
    }

    bool FindVisualRedefineRow(
        int mouseX,
        int mouseY,
        unsigned int* rowFound)
    {
        // UI_FRONTEND_LIST_REDEFINE_KEYS_MENU is rooted at (40,115).
        // Its generated UI_REDEFINER_MOUSE_AREA child is (-40,0),
        // 600x24, with the list's exact 26-pixel vertical step.
        for (unsigned int row = 0; row != 9; ++row)
        {
            FableUiVector2 origin = {};
            ResolveVisualGeneratedChildOrigin(
                40.0f,
                115.0f,
                0.0f,
                static_cast<float>(row * 26),
                -40.0f,
                0.0f,
                &origin);
            const int rowLeft = static_cast<int>(origin.x);
            const int rowTop = static_cast<int>(origin.y);
            if (
                mouseX >= rowLeft &&
                mouseX < rowLeft + 600 &&
                mouseY >= rowTop &&
                mouseY < rowTop + 24)
            {
                *rowFound = row;
                return true;
            }
        }
        return false;
    }

    bool FABLE_FASTCALL RunVisualFrontendPostMovieStep(
        void*,
        FableFrontendPostMovieStep step)
    {
        g_FrontendPostMovieStepMask |=
            1UL << static_cast<unsigned long>(step);
        return true;
    }

    const char* IntegerResource(int identifier)
    {
        return reinterpret_cast<const char*>(
            static_cast<unsigned long>(identifier));
    }

    void PaintBootArtwork(FableWindow window)
    {
        FablePaint paint = {};
        FableDeviceContext destination = BeginPaint(window, &paint);

        FableRectangle client = {};
        GetClientRect(window, &client);
        const int clientWidth = client.right - client.left;
        const int clientHeight = client.bottom - client.top;

        FableBrush background = static_cast<FableBrush>(
            GetStockObject(kBlackBrush));
        FillRect(destination, &client, background);

        if (
            g_VisualFrontendVisible &&
            g_BootArtwork != 0 &&
            g_BootArtworkInfo.width > 0 &&
            g_BootArtworkInfo.height > 0)
        {
            const int drawWidth = clientWidth;
            const int drawHeight = clientHeight;
            const int drawLeft = 0;
            const int drawTop = 0;

            FableDeviceContext source = CreateCompatibleDC(destination);
            FableGraphicsObject previous =
                SelectObject(source, g_BootArtwork);
            SetStretchBltMode(destination, kHalftoneStretch);
            SetBrushOrgEx(destination, 0, 0, 0);
            StretchBlt(
                destination,
                drawLeft,
                drawTop,
                drawWidth,
                drawHeight,
                source,
                0,
                0,
                g_BootArtworkInfo.width,
                g_BootArtworkInfo.height,
                kCopySource);
            SelectObject(source, previous);
            DeleteDC(source);
        }

        EndPaint(window, &paint);
    }

    bool PaintBootArtworkD3D9(FableWindow window)
    {
        if (!FableIsVisualD3D9Active())
            return false;

        FableRectangle client = {};
        GetClientRect(window, &client);
        const bool presented = g_VisualFrontendVisible
            ? FableRenderVisualD3D9(
                client.right - client.left,
                client.bottom - client.top)
            : FablePresentVisualD3D9Black();
        if (!presented)
        {
            return false;
        }

        FablePaint paint = {};
        BeginPaint(window, &paint);
        EndPaint(window, &paint);
        return true;
    }

    void RevealVisualFrontend(FableWindow window)
    {
        g_VisualFrontendVisible = true;
        if (FableIsVisualD3D9Active())
        {
            FableRectangle client = {};
            if (
                GetClientRect(window, &client) &&
                FableRenderVisualD3D9(
                    client.right - client.left,
                    client.bottom - client.top))
            {
                return;
            }
        }

        InvalidateRect(window, 0, 1);
        UpdateWindow(window);
    }

    void CopyVisualDetailValues(
        unsigned int destination[3][10],
        const unsigned int source[3][10])
    {
        memcpy(destination, source, sizeof(g_VisualDetailValues));
    }

    void CopyVisualRedefineKeys(
        unsigned int destination[9],
        const unsigned int source[9])
    {
        memcpy(destination, source, sizeof(g_VisualRedefineKeys));
    }

    void SyncVisualRedefineKeys()
    {
        for (unsigned int row = 0; row != 9; ++row)
        {
            FableSetVisualFrontendRedefineKey(
                row,
                g_VisualRedefineKeys[row]);
        }
        FableSetVisualFrontendRedefineSelection(
            g_VisualRedefineSelection);
    }

    void ResetVisualRedefineKeys(bool wasd)
    {
        // UI_RESET_WASD action 311 dispatches
        // CUserProfileManager::ResetAssignedInputsWASD @ 0x00408820.
        // UI_RESET action 284 dispatches ResetAssignedInputs @ 0x004085F0,
        // whose shipped GDD scheme uses the four arrow-key controls.
        CopyVisualRedefineKeys(
            g_VisualRedefineKeys,
            wasd
                ? kVisualRedefineDefaults
                : kVisualRedefineArrowDefaults);
        g_VisualRedefineSelection = 0;
        SyncVisualRedefineKeys();
    }

    void SyncVisualDetailScreen(unsigned int screen)
    {
        if (screen < 1 || screen > 3)
            return;
        const unsigned int screenIndex = screen - 1;
        for (
            unsigned int row = 0;
            row != kVisualDetailRowCounts[screenIndex];
            ++row)
        {
            FableSetVisualFrontendDetailOptionValue(
                screen,
                row,
                g_VisualDetailValues[screenIndex][row]);
        }
    }

    void BeginVisualDetailScreen(unsigned int screen)
    {
        if (screen >= 1 && screen <= 3)
        {
            CopyVisualDetailValues(
                g_VisualDetailValues,
                g_VisualDetailSavedValues);
            CopyVisualDetailValues(
                g_VisualDetailEntryValues,
                g_VisualDetailValues);
            SyncVisualDetailScreen(screen);
        }
        else if (screen == 4)
        {
            CopyVisualRedefineKeys(
                g_VisualRedefineKeys,
                g_VisualRedefineSavedKeys);
            CopyVisualRedefineKeys(
                g_VisualRedefineEntryKeys,
                g_VisualRedefineKeys);
            g_VisualRedefineSelection = 0;
            SyncVisualRedefineKeys();
        }
    }

    void CancelVisualDetailScreen()
    {
        if (g_VisualDetailScreen >= 1 && g_VisualDetailScreen <= 3)
        {
            CopyVisualDetailValues(
                g_VisualDetailValues,
                g_VisualDetailEntryValues);
            SyncVisualDetailScreen(g_VisualDetailScreen);
        }
        else if (g_VisualDetailScreen == 4)
        {
            CopyVisualRedefineKeys(
                g_VisualRedefineKeys,
                g_VisualRedefineEntryKeys);
            g_VisualRedefineSelection = 0;
            SyncVisualRedefineKeys();
        }
    }

    void ApplyVisualDetailScreen()
    {
        if (g_VisualDetailScreen >= 1 && g_VisualDetailScreen <= 3)
        {
            CopyVisualDetailValues(
                g_VisualDetailSavedValues,
                g_VisualDetailValues);
        }
        else if (g_VisualDetailScreen == 4)
        {
            CopyVisualRedefineKeys(
                g_VisualRedefineSavedKeys,
                g_VisualRedefineKeys);
            g_VisualRedefineSelection = 0;
            FableSetVisualFrontendRedefineSelection(0);
        }
    }

    unsigned int VisualVirtualKeyToRedefineValue(
        FableWordParameter virtualKey)
    {
        if (virtualKey >= 'A' && virtualKey <= 'Z')
        {
            if (virtualKey == 'Q')
                return 5;
            if (virtualKey == 'E')
                return 6;
            return 9 + static_cast<unsigned int>(virtualKey - 'A');
        }
        if (virtualKey >= '0' && virtualKey <= '9')
            return 45 + static_cast<unsigned int>(virtualKey - '0');
        switch(virtualKey)
        {
        case 0x09:
            return 4;
        case 0x20:
            return 35;
        case 0x10:
            return 36;
        case 0x11:
            return 37;
        case 0x12:
            return 38;
        case 0x0D:
            return 39;
        case 0x08:
            return 40;
        case 0x26:
            return 41;
        case 0x28:
            return 42;
        case 0x25:
            return 43;
        case 0x27:
            return 44;
        }
        return 0xFFFFFFFFu;
    }

    bool VisualRedefineRowsMayShare(
        unsigned int first,
        unsigned int second)
    {
        // Retail AreAllowedToCoexist permits the Flourish, Run, and
        // first-person-targeting actions to share a control.  Movement action
        // 60 expands into four CKeyRedefiner rows ahead of these actions.
        return first >= 6 && first <= 8 &&
            second >= 6 && second <= 8;
    }

    bool ApplyVisualRedefinition(unsigned int keyValue)
    {
        if (
            g_VisualDetailScreen != 4 ||
            g_VisualRedefineSelection == 0 ||
            g_VisualRedefineSelection > 9 ||
            keyValue >= 55 ||
            keyValue == 7)
        {
            return false;
        }
        const unsigned int selected =
            g_VisualRedefineSelection - 1;
        for (unsigned int row = 0; row != 9; ++row)
        {
            if (
                row != selected &&
                g_VisualRedefineKeys[row] == keyValue &&
                !VisualRedefineRowsMayShare(row, selected))
            {
                // CRedefinerList::NotifyOfRedefinition invokes
                // ClearDuplicateDefinitions on incompatible aliases.
                g_VisualRedefineKeys[row] = 8;
                FableSetVisualFrontendRedefineKey(row, 8);
            }
        }
        g_VisualRedefineKeys[selected] = keyValue;
        FableSetVisualFrontendRedefineKey(selected, keyValue);
        g_VisualRedefineSelection = 0;
        FableSetVisualFrontendRedefineSelection(0);
        return true;
    }

    bool AdjustVisualDetailControl(
        unsigned int screen,
        int mouseX,
        int mouseY)
    {
        if (screen < 1 || screen > 3)
            return false;
        const unsigned int screenIndex = screen - 1;
        for (
            unsigned int row = 0;
            row != kVisualDetailRowCounts[screenIndex];
            ++row)
        {
            const int rowTop = kVisualDetailRowY[screenIndex][row] - 3;
            if (mouseY < rowTop || mouseY >= rowTop + 30)
                continue;
            unsigned int& value =
                g_VisualDetailValues[screenIndex][row];
            const unsigned int valueCount =
                kVisualDetailValueCounts[screenIndex][row];
            const unsigned int previousValue = value;
            if (mouseX >= 300 && mouseX < 340)
            {
                if (value != 0)
                    --value;
            }
            else if (mouseX >= 462 && mouseX < 502)
            {
                if (value + 1 < valueCount)
                    ++value;
            }
            else if (
                mouseX >= 340 &&
                mouseX < 462 &&
                valueCount > 1)
            {
                value = static_cast<unsigned int>(
                    (mouseX - 340) * (valueCount - 1) / 121);
            }
            else
            {
                return false;
            }
            if (value == previousValue)
                return false;
            FableSetVisualFrontendDetailOptionValue(
                screen,
                row,
                value);
            return true;
        }
        return false;
    }

    bool StartRetailBootMovie()
    {
        if (
            !g_RetailBootSequenceActive ||
            g_RetailBootMovieIndex >= kRetailBootMovieCount)
        {
            return false;
        }

        g_RetailVideoReachedRunningState = false;
        return FableStartRetailVideo(
            g_RetailVideoWindow,
            g_RetailVideoInstance,
            kRetailBootMovies[g_RetailBootMovieIndex]);
    }

    FableResult FABLE_STDCALL VisualBootWindowProcedure(
        FableWindow window,
        FableUint message,
        FableWordParameter wordParameter,
        FableLongParameter longParameter)
    {
        switch (message)
        {
        case kMessageSize:
        {
            const fable_i32 clientWidth =
                static_cast<fable_i32>(
                    longParameter & 0xFFFF);
            const fable_i32 clientHeight =
                static_cast<fable_i32>(
                    (longParameter >> 16) & 0xFFFF);
            if (FableIsRetailVideoActive())
            {
                FableResizeRetailVideo(
                    clientWidth,
                    clientHeight);
            }
            if (
                FableIsVisualD3D9Active() &&
                clientWidth > 0 &&
                clientHeight > 0 &&
                FableResizeVisualD3D9(
                    clientWidth,
                    clientHeight))
            {
                if (g_VisualFrontendVisible)
                {
                    FableRenderVisualD3D9(
                        clientWidth,
                        clientHeight);
                }
                else
                {
                    FablePresentVisualD3D9Black();
                }
            }
            return 0;
        }

        case kMessagePaint:
            if (!PaintBootArtworkD3D9(window))
                PaintBootArtwork(window);
            return 0;

        case kMessageKeyDown:
            if (
                wordParameter == kEscapeKey &&
                !g_RetailVideoEscapePressed &&
                FableIsRetailVideoActive())
            {
                g_RetailVideoEscapePressed = true;
                FableSkipRetailVideo();
                return 0;
            }
            if (
                !FableIsRetailVideoActive() &&
                g_VisualDetailScreen == 4 &&
                g_VisualRedefineSelection != 0)
            {
                if (wordParameter == kEscapeKey)
                {
                    // Retail event 0x21 with Escape selected calls
                    // CKeyRedefiner::CancelSelection.
                    g_VisualRedefineSelection = 0;
                    FableSetVisualFrontendRedefineSelection(0);
                    RevealVisualFrontend(window);
                    return 0;
                }
                const unsigned int keyValue =
                    VisualVirtualKeyToRedefineValue(wordParameter);
                if (ApplyVisualRedefinition(keyValue))
                {
                    RevealVisualFrontend(window);
                    return 0;
                }
            }
            if (
                wordParameter == kEscapeKey &&
                !FableIsRetailVideoActive())
            {
                if (g_VisualQuitPromptActive)
                {
                    // Retail action 86: GotoPreviousScreen(false).
                    g_VisualQuitPromptActive = false;
                    g_VisualMainMenuActive = true;
                    FableSetVisualFrontendQuitPrompt(false);
                    FableSetVisualFrontendMainMenu(true);
                    RevealVisualFrontend(window);
                    return 0;
                }
                if (g_VisualDetailScreen != 0)
                {
                    // Each detail screen was pushed by GotoNextScreen;
                    // action 86 reverts its active sliders and returns to
                    // UI_FRONTEND_OPTIONS_SUB_MENU.
                    CancelVisualDetailScreen();
                    g_VisualDetailScreen = 0;
                    g_VisualOptionsMenuActive = true;
                    FableSetVisualFrontendDetailScreen(0);
                    FableSetVisualFrontendOptionsMenu(true);
                    RevealVisualFrontend(window);
                    return 0;
                }
                if (g_VisualOptionsMenuActive)
                {
                    // UI_BACK (#563) also dispatches retail action 86.
                    g_VisualOptionsMenuActive = false;
                    g_VisualMainMenuActive = true;
                    FableSetVisualFrontendOptionsMenu(false);
                    FableSetVisualFrontendMainMenu(true);
                    RevealVisualFrontend(window);
                    return 0;
                }
            }
            break;

        case kMessageKeyUp:
            if (wordParameter == kEscapeKey)
            {
                g_RetailVideoEscapePressed = false;
                return 0;
            }
            break;

        case kMessageRightButtonUp:
        case kMessageMiddleButtonUp:
            if (
                !FableIsRetailVideoActive() &&
                g_VisualDetailScreen == 4 &&
                g_VisualRedefineSelection != 0 &&
                ApplyVisualRedefinition(
                    message == kMessageRightButtonUp ? 3 : 2))
            {
                RevealVisualFrontend(window);
                return 0;
            }
            break;

        case kMessageLeftButtonUp:
            if (
                !FableIsRetailVideoActive() &&
                g_VisualDetailScreen == 4 &&
                g_VisualRedefineSelection != 0 &&
                ApplyVisualRedefinition(1))
            {
                RevealVisualFrontend(window);
                return 0;
            }
            if (
                g_VisualFrontendVisible &&
                !FableIsRetailVideoActive() &&
                (g_VisualMainMenuActive ||
                 g_VisualOptionsMenuActive ||
                 g_VisualDetailScreen != 0 ||
                 g_VisualQuitPromptActive))
            {
                FableRectangle client = {};
                if (!GetClientRect(window, &client))
                    break;
                const int clientWidth = client.right - client.left;
                const int clientHeight = client.bottom - client.top;
                if (clientWidth <= 0 || clientHeight <= 0)
                    break;
                const int mouseX =
                    static_cast<int>(longParameter & 0xFFFF) *
                    640 / clientWidth;
                const int mouseY =
                    static_cast<int>(
                        (longParameter >> 16) & 0xFFFF) *
                    480 / clientHeight;
                if (g_VisualOptionsMenuActive)
                {
                    if (
                        mouseX >= 20 &&
                        mouseX < 270 &&
                        mouseY >= 420 &&
                        mouseY < 450)
                    {
                        // Retail action 86: GotoPreviousScreen(false).
                        g_VisualOptionsMenuActive = false;
                        g_VisualMainMenuActive = true;
                        FableSetVisualFrontendOptionsMenu(false);
                        FableSetVisualFrontendMainMenu(true);
                        RevealVisualFrontend(window);
                        return 0;
                    }
                    unsigned int row = 0;
                    if (FindVisualOptionsMenuRow(mouseX, mouseY, &row))
                    {
                        // Retail dispatcher/Init2 map:
                        // row 0 action 9   -> key 0x01 Gameplay
                        // row 1 action 13  -> key 0x05 Video
                        // row 2 action 12  -> key 0x04 Audio
                        // row 3 action 283 -> key 0x16 Redefine
                        const unsigned int detailScreens[4] = {
                            1, 3, 2, 4
                        };
                        g_VisualOptionsSelection = row;
                        g_VisualOptionsMenuActive = false;
                        g_VisualDetailScreen = detailScreens[row];
                        g_VisualRedefineHover = 0;
                        g_VisualRedefineResetHover = 0;
                        BeginVisualDetailScreen(
                            g_VisualDetailScreen);
                        FableSetVisualFrontendOptionsMenu(false);
                        FableSetVisualFrontendDetailScreen(
                            g_VisualDetailScreen);
                        RevealVisualFrontend(window);
                        return 0;
                    }
                }
                else if (g_VisualDetailScreen != 0)
                {
                    if (
                        mouseX >= 362 &&
                        mouseX < 618 &&
                        mouseY >= 424 &&
                        mouseY < 480)
                    {
                        // UI_ACCEPT action 87 saves the current profile and
                        // returns to the previous screen.
                        ApplyVisualDetailScreen();
                        g_VisualDetailScreen = 0;
                        g_VisualRedefineHover = 0;
                        g_VisualRedefineResetHover = 0;
                        g_VisualOptionsMenuActive = true;
                        FableSetVisualFrontendDetailScreen(0);
                        FableSetVisualFrontendOptionsMenu(true);
                        RevealVisualFrontend(window);
                        return 0;
                    }
                    if (
                        mouseX >= 20 &&
                        mouseX < 276 &&
                        mouseY >= 424 &&
                        mouseY < 480)
                    {
                        // UI_CANCEL action 86 restores activation values.
                        CancelVisualDetailScreen();
                        g_VisualDetailScreen = 0;
                        g_VisualRedefineHover = 0;
                        g_VisualRedefineResetHover = 0;
                        g_VisualOptionsMenuActive = true;
                        FableSetVisualFrontendDetailScreen(0);
                        FableSetVisualFrontendOptionsMenu(true);
                        RevealVisualFrontend(window);
                        return 0;
                    }
                    if (
                        g_VisualDetailScreen <= 3 &&
                        mouseX >= 192 &&
                        mouseX < 448 &&
                        mouseY >= 384 &&
                        mouseY < 424)
                    {
                        // Actions 324/325/326 set the profile group defaults
                        // and reset each active slider from that profile.
                        const unsigned int screenIndex =
                            g_VisualDetailScreen - 1;
                        for (
                            unsigned int row = 0;
                            row != kVisualDetailRowCounts[screenIndex];
                            ++row)
                        {
                            g_VisualDetailValues[screenIndex][row] =
                                kVisualDetailDefaults[screenIndex][row];
                        }
                        SyncVisualDetailScreen(g_VisualDetailScreen);
                        RevealVisualFrontend(window);
                        return 0;
                    }
                    if (g_VisualDetailScreen == 4)
                    {
                        if (
                            mouseY >= 389 &&
                            mouseY < 429)
                        {
                            // UI_HELPERS_REDEFINE contributes parent y=20.
                            // Its reset controls are at local (0/320,385);
                            // their table graphics begin 16 pixels above the
                            // clickable parent. Actions are 311 (WASD) and
                            // 284 (arrows).
                            ResetVisualRedefineKeys(mouseX < 320);
                            RevealVisualFrontend(window);
                            return 0;
                        }
                        unsigned int row = 0;
                        if (FindVisualRedefineRow(
                                mouseX,
                                mouseY,
                                &row))
                        {
                            // CKeyRedefiner::OnLeftUnclicked installs the
                            // one active capture target and replaces this
                            // row's value with TEXT_GUI_PRESS_CONTROL.
                            g_VisualRedefineSelection = row + 1;
                            FableSetVisualFrontendRedefineSelection(
                                g_VisualRedefineSelection);
                            RevealVisualFrontend(window);
                            return 0;
                        }
                    }
                    if (AdjustVisualDetailControl(
                            g_VisualDetailScreen,
                            mouseX,
                            mouseY))
                    {
                        // Retail profile setters apply the in-memory value
                        // immediately; persistence waits for action 87.
                        RevealVisualFrontend(window);
                        return 0;
                    }
                }
                else if (g_VisualQuitPromptActive)
                {
                    if (
                        mouseX >= 20 &&
                        mouseX < 276 &&
                        mouseY >= 389 &&
                        mouseY < 453)
                    {
                        // UI_FRONTEND_QUIT_NO dispatches retail action 86.
                        g_VisualQuitPromptActive = false;
                        g_VisualMainMenuActive = true;
                        FableSetVisualFrontendQuitPrompt(false);
                        FableSetVisualFrontendMainMenu(true);
                        RevealVisualFrontend(window);
                        return 0;
                    }
                    if (
                        mouseX >= 362 &&
                        mouseX < 618 &&
                        mouseY >= 389 &&
                        mouseY < 453)
                    {
                        // Retail action 296 calls CMainGameComponent::SetQuit.
                        // Leaving the message loop performs the same ordered
                        // video, D3D9, and window-class shutdown here.
                        PostQuitMessage(0);
                        return 0;
                    }
                }
                else if (g_VisualMainMenuActive)
                {
                    unsigned int row = 0;
                    if (FindVisualMainMenuRow(mouseX, mouseY, &row))
                    {
                        g_VisualMainMenuSelection = row;
                        if (row == 2)
                        {
                            // Action 297 -> used-key 0x18 ->
                            // UI_FRONTEND_OPTIONS_SUB_MENU.
                            g_VisualMainMenuActive = false;
                            g_VisualOptionsMenuActive = true;
                            g_VisualOptionsSelection = 0;
                            g_VisualOptionsBackHovered = false;
                            FableSetVisualFrontendOptionsMenu(true);
                            RevealVisualFrontend(window);
                            return 0;
                        }
                        if (row == 6)
                        {
                            // Action 314 -> used-key 0x1a ->
                            // UI_FRONTEND_QUIT_PROMPT.
                            g_VisualMainMenuActive = false;
                            g_VisualQuitPromptActive = true;
                            g_VisualQuitHover = 0;
                            FableSetVisualFrontendQuitPrompt(true);
                            RevealVisualFrontend(window);
                            return 0;
                        }
                    }
                }
            }
            if (
                g_VisualFrontendVisible &&
                !g_VisualMainMenuActive &&
                !g_VisualOptionsMenuActive &&
                g_VisualDetailScreen == 0 &&
                !g_VisualQuitPromptActive &&
                !FableIsRetailVideoActive())
            {
                // UI_FRONTEND_BUTTON_INVISIBLE (#625) dispatches action 229
                // on left-button release.  The resulting first-menu root is
                // UI_FRONTEND_MAIN_MENU (#212).
                g_VisualMainMenuActive = true;
                g_VisualMainMenuSelection = 0;
                FableSetVisualFrontendMainMenu(true);
                RevealVisualFrontend(window);
                return 0;
            }
            break;

        case kMessageMouseMove:
        {
            if (
                !g_VisualMainMenuActive &&
                !g_VisualOptionsMenuActive &&
                g_VisualDetailScreen == 0 &&
                !g_VisualQuitPromptActive)
                break;
            FableRectangle client = {};
            if (!GetClientRect(window, &client))
                break;
            const int clientWidth = client.right - client.left;
            const int clientHeight = client.bottom - client.top;
            if (clientWidth <= 0 || clientHeight <= 0)
                break;
            const int mouseX =
                static_cast<int>(longParameter & 0xFFFF) *
                640 / clientWidth;
            const int mouseY =
                static_cast<int>((longParameter >> 16) & 0xFFFF) *
                480 / clientHeight;
            if (g_VisualQuitPromptActive)
            {
                unsigned int hover = 0;
                if (
                    mouseX >= 20 &&
                    mouseX < 276 &&
                    mouseY >= 389 &&
                    mouseY < 453)
                {
                    hover = 1;
                }
                else if (
                    mouseX >= 362 &&
                    mouseX < 618 &&
                    mouseY >= 389 &&
                    mouseY < 453)
                {
                    hover = 2;
                }
                if (g_VisualQuitHover != hover)
                {
                    g_VisualQuitHover = hover;
                    FableSetVisualFrontendQuitHover(hover);
                    RevealVisualFrontend(window);
                }
                return 0;
            }
            if (g_VisualOptionsMenuActive)
            {
                const bool backHovered =
                    mouseX >= 20 &&
                    mouseX < 270 &&
                    mouseY >= 420 &&
                    mouseY < 450;
                if (
                    g_VisualOptionsBackHovered != backHovered)
                {
                    g_VisualOptionsBackHovered = backHovered;
                    FableSetVisualFrontendOptionsBackHovered(
                        backHovered);
                    RevealVisualFrontend(window);
                }
                unsigned int row = 0;
                if (FindVisualOptionsMenuRow(mouseX, mouseY, &row))
                {
                    if (g_VisualOptionsSelection != row)
                    {
                        g_VisualOptionsSelection = row;
                        FableSetVisualFrontendOptionsSelection(row);
                        RevealVisualFrontend(window);
                    }
                }
                return 0;
            }
            if (g_VisualDetailScreen != 0)
            {
                if (g_VisualDetailScreen == 4)
                {
                    unsigned int row = 0;
                    unsigned int hover = 0;
                    if (FindVisualRedefineRow(mouseX, mouseY, &row))
                        hover = row + 1;
                    if (g_VisualRedefineHover != hover)
                    {
                        g_VisualRedefineHover = hover;
                        FableSetVisualFrontendRedefineHover(hover);
                        RevealVisualFrontend(window);
                    }
                    unsigned int resetHover = 0;
                    if (mouseY >= 389 && mouseY < 453)
                        resetHover = mouseX < 320 ? 1 : 2;
                    if (g_VisualRedefineResetHover != resetHover)
                    {
                        g_VisualRedefineResetHover = resetHover;
                        FableSetVisualFrontendRedefineResetHover(resetHover);
                        RevealVisualFrontend(window);
                    }
                }
                return 0;
            }
            unsigned int row = 0;
            if (FindVisualMainMenuRow(mouseX, mouseY, &row))
            {
                if (g_VisualMainMenuSelection != row)
                {
                    g_VisualMainMenuSelection = row;
                    FableSetVisualFrontendMainMenuSelection(row);
                    RevealVisualFrontend(window);
                }
            }
            return 0;
        }

        case kMessageTimer:
            if (
                wordParameter == kFrontendAnimationTimer &&
                g_VisualFrontendVisible)
            {
                InvalidateRect(window, 0, 0);
                return 0;
            }
            if (wordParameter == kRetailVideoTimer)
            {
                if (
                    !g_RetailVideoReachedRunningState &&
                    FableRetailVideoHasAdvanced())
                {
                    g_RetailVideoReachedRunningState = true;
                    SetWindowTextA(
                        window,
                        g_RetailBootSequenceActive
                            ? (
                                FableIsRetailVideoUsingUpscaledSource()
                                    ? kRetailBootUpscaledPlayingWindowTitles[
                                        g_RetailBootMovieIndex]
                                    : kRetailBootPlayingWindowTitles[
                                        g_RetailBootMovieIndex]
                            )
                            : (
                                FableIsRetailVideoUsingUpscaledSource()
                                    ? kRetailVideoUpscaledPlayingWindowTitle
                                    : kRetailVideoPlayingWindowTitle
                            ));
                }

                const FableRetailVideoProcessResult result =
                    FableProcessRetailVideo();
                if (result == FableRetailVideoCompleted)
                {
                    if (
                        g_RetailBootSequenceActive &&
                        ++g_RetailBootMovieIndex <
                            kRetailBootMovieCount)
                    {
                        if (StartRetailBootMovie())
                        {
                            SetWindowTextA(
                                window,
                                kRetailVideoStartingWindowTitle);
                        }
                        else
                        {
                            KillTimer(window, kRetailVideoTimer);
                            SetWindowTextA(
                                window,
                                FableGetRetailVideoStatus());
                        }
                    }
                    else
                    {
                        KillTimer(window, kRetailVideoTimer);
                        FableShutdownRetailVideo();
                        RevealVisualFrontend(window);
                        bool frontendStartupReady = true;
                        if (g_RetailBootSequenceActive)
                        {
                            g_FrontendPostMovieStepMask = 0;
                            FableFrontendPostMovieServices services = {
                                true,
                                &RunVisualFrontendPostMovieStep,
                                window
                            };
                            frontendStartupReady =
                                FableRunFrontendPostMovieStartup(
                                    services) &&
                                g_FrontendPostMovieStepMask == 0x1FF;
                        }
                        SetWindowTextA(
                            window,
                            g_RetailBootSequenceActive
                                ? (
                                    frontendStartupReady
                                        ? kRetailVideoSequenceCompleteWindowTitle
                                        : kRetailVideoSequenceFailedWindowTitle
                                )
                                : kRetailVideoCompleteWindowTitle);
                    }
                }
                else if (result == FableRetailVideoFailed)
                {
                    KillTimer(window, kRetailVideoTimer);
                    SetWindowTextA(
                        window,
                        FableGetRetailVideoStatus());
                    FableShutdownRetailVideo();
                    RevealVisualFrontend(window);
                }
            }
            return 0;

        case kMessageEraseBackground:
            return 1;

        case kMessageDestroy:
            KillTimer(window, kRetailVideoTimer);
            KillTimer(window, kFrontendAnimationTimer);
            FableShutdownRetailVideo();
            FableShutdownVisualD3D9();
            PostQuitMessage(0);
            return 0;
        }

        return DefWindowProcA(
            window,
            message,
            wordParameter,
            longParameter);
    }
}

void FABLE_FASTCALL FableSetVisualProgressDisplayState(
    bool present,
    bool active)
{
    g_RetailProgressDisplayPresent = present;
    g_RetailProgressDisplayActive = active;
}

bool FABLE_FASTCALL FableIsRetailVisualAssetEmbedded()
{
#if defined(FABLETLC_RETAIL_FRONTEND_ARTWORK)
    return true;
#else
    return false;
#endif
}

bool FABLE_FASTCALL FableShouldPlayBootVideos(const char* commandLine)
{
    if (
        commandLine == 0 ||
        strstr(commandLine, "--skip-boot-videos") != 0)
    {
        return false;
    }

    return
        strstr(commandLine, "--retail-video") != 0 ||
        strstr(commandLine, "--buff-jesus") != 0;
}

long FABLE_FASTCALL FableRunVisualBootCheckpoint(
    FableInstanceHandle instance,
    char* commandLine,
    int showCommand)
{
    g_BootArtwork = static_cast<FableBitmap>(LoadImageA(
        instance,
        IntegerResource(kBootArtworkResource),
        kImageBitmap,
        0,
        0,
        kLoadCreatedDibSection));
    if (g_BootArtwork == 0)
        return 1;

    GetObjectA(
        g_BootArtwork,
        sizeof(g_BootArtworkInfo),
        &g_BootArtworkInfo);
#if defined(FABLETLC_RETAIL_FRONTEND_ARTWORK)
    g_BootTitleArtwork = static_cast<FableBitmap>(LoadImageA(
        instance,
        IntegerResource(kBootTitleResource),
        kImageBitmap,
        0,
        0,
        kLoadCreatedDibSection));
    if (g_BootTitleArtwork == 0)
    {
        DeleteObject(g_BootArtwork);
        g_BootArtwork = 0;
        return 1;
    }
    GetObjectA(
        g_BootTitleArtwork,
        sizeof(g_BootTitleArtworkInfo),
        &g_BootTitleArtworkInfo);
#if defined(FABLETLC_RETAIL_FRONTEND_ANIMATION)
    g_BootForestArtwork = static_cast<FableBitmap>(LoadImageA(
        instance,
        IntegerResource(103),
        kImageBitmap,
        0,
        0,
        kLoadCreatedDibSection));
    g_BootSunbeamArtwork = static_cast<FableBitmap>(LoadImageA(
        instance,
        IntegerResource(104),
        kImageBitmap,
        0,
        0,
        kLoadCreatedDibSection));
    if (g_BootForestArtwork == 0 || g_BootSunbeamArtwork == 0)
    {
        DeleteObject(g_BootSunbeamArtwork);
        DeleteObject(g_BootForestArtwork);
        DeleteObject(g_BootTitleArtwork);
        DeleteObject(g_BootArtwork);
        g_BootSunbeamArtwork = 0;
        g_BootForestArtwork = 0;
        g_BootTitleArtwork = 0;
        g_BootArtwork = 0;
        return 1;
    }
    GetObjectA(
        g_BootForestArtwork,
        sizeof(g_BootForestArtworkInfo),
        &g_BootForestArtworkInfo);
    GetObjectA(
        g_BootSunbeamArtwork,
        sizeof(g_BootSunbeamArtworkInfo),
        &g_BootSunbeamArtworkInfo);
#endif
#endif

    if (
        commandLine != 0 &&
        strstr(commandLine, "--verify-visual-resource") != 0)
    {
        DeleteObject(g_BootArtwork);
        g_BootArtwork = 0;
#if defined(FABLETLC_RETAIL_FRONTEND_ARTWORK)
        DeleteObject(g_BootTitleArtwork);
        g_BootTitleArtwork = 0;
#if defined(FABLETLC_RETAIL_FRONTEND_ANIMATION)
        DeleteObject(g_BootSunbeamArtwork);
        DeleteObject(g_BootForestArtwork);
        g_BootSunbeamArtwork = 0;
        g_BootForestArtwork = 0;
#endif
#endif
        return 0;
    }

    const bool retailVideoRequested =
        FableShouldPlayBootVideos(commandLine);
    g_VisualFrontendVisible = !retailVideoRequested;
    g_VisualMainMenuActive = false;
    g_VisualMainMenuSelection = 0;
    g_VisualOptionsMenuActive = false;
    g_VisualOptionsSelection = 0;
    g_VisualOptionsBackHovered = false;
    g_VisualDetailScreen = 0;
    g_VisualRedefineHover = 0;
    g_VisualRedefineResetHover = 0;
    g_VisualRedefineSelection = 0;
    CopyVisualRedefineKeys(
        g_VisualRedefineKeys,
        kVisualRedefineDefaults);
    CopyVisualRedefineKeys(
        g_VisualRedefineEntryKeys,
        kVisualRedefineDefaults);
    CopyVisualRedefineKeys(
        g_VisualRedefineSavedKeys,
        kVisualRedefineDefaults);
    CopyVisualDetailValues(
        g_VisualDetailValues,
        kVisualDetailDefaults);
    CopyVisualDetailValues(
        g_VisualDetailEntryValues,
        kVisualDetailDefaults);
    CopyVisualDetailValues(
        g_VisualDetailSavedValues,
        kVisualDetailDefaults);
    g_VisualQuitPromptActive = false;
    g_VisualQuitHover = 0;

    FableWindowClass windowClass = {};
    windowClass.size = sizeof(windowClass);
    windowClass.style =
        kClassRedrawHorizontal | kClassRedrawVertical;
    windowClass.procedure = VisualBootWindowProcedure;
    windowClass.instance = instance;
    windowClass.cursor =
#if defined(FABLETLC_RETAIL_FRONTEND_ARTWORK)
        LoadCursorA(instance, IntegerResource(kBootPointerResource));
    if (windowClass.cursor == 0)
    {
        windowClass.cursor =
            LoadCursorA(0, IntegerResource(kArrowCursor));
    }
#else
        LoadCursorA(0, IntegerResource(kArrowCursor));
#endif
    windowClass.background = static_cast<FableBrush>(
        GetStockObject(kBlackBrush));
    windowClass.className = kWindowClassName;

    if (RegisterClassExA(&windowClass) == 0)
    {
        DeleteObject(g_BootArtwork);
        g_BootArtwork = 0;
#if defined(FABLETLC_RETAIL_FRONTEND_ARTWORK)
        DeleteObject(g_BootTitleArtwork);
        g_BootTitleArtwork = 0;
#if defined(FABLETLC_RETAIL_FRONTEND_ANIMATION)
        DeleteObject(g_BootSunbeamArtwork);
        DeleteObject(g_BootForestArtwork);
        g_BootSunbeamArtwork = 0;
        g_BootForestArtwork = 0;
#endif
#endif
        return 2;
    }

    FableRectangle windowBounds = {0, 0, 1280, 720};
    AdjustWindowRectEx(
        &windowBounds,
        kOverlappedWindow,
        0,
        0);

    const char* windowTitle = kWindowTitle;
    if (g_RetailProgressDisplayPresent)
    {
#if defined(FABLETLC_RETAIL_FRONTEND_ARTWORK)
        windowTitle = g_RetailProgressDisplayActive
            ? kRetailAssetActiveWindowTitle
            : kRetailAssetReadyWindowTitle;
#else
        windowTitle = g_RetailProgressDisplayActive
            ? kProgressActiveWindowTitle
            : kProgressReadyWindowTitle;
#endif
    }

    FableWindow window = CreateWindowExA(
        0,
        kWindowClassName,
        windowTitle,
        kOverlappedWindow,
        kUseDefaultPosition,
        kUseDefaultPosition,
        windowBounds.right - windowBounds.left,
        windowBounds.bottom - windowBounds.top,
        0,
        0,
        instance,
        0);
    if (window == 0)
    {
        UnregisterClassA(kWindowClassName, instance);
        DeleteObject(g_BootArtwork);
        g_BootArtwork = 0;
#if defined(FABLETLC_RETAIL_FRONTEND_ARTWORK)
        DeleteObject(g_BootTitleArtwork);
        g_BootTitleArtwork = 0;
#if defined(FABLETLC_RETAIL_FRONTEND_ANIMATION)
        DeleteObject(g_BootSunbeamArtwork);
        DeleteObject(g_BootForestArtwork);
        g_BootSunbeamArtwork = 0;
        g_BootForestArtwork = 0;
#endif
#endif
        return 3;
    }

#if defined(FABLETLC_RETAIL_FRONTEND_MENU)
    g_BootMenuArtwork = static_cast<FableBitmap>(LoadImageA(
        instance,
        IntegerResource(kBootMenuResource),
        kImageBitmap,
        0,
        0,
        kLoadCreatedDibSection));
    g_BootBuffJesusMenuArtwork = static_cast<FableBitmap>(LoadImageA(
        instance,
        IntegerResource(kBootBuffJesusMenuResource),
        kImageBitmap,
        0,
        0,
        kLoadCreatedDibSection));
    g_BootCoastalArtwork = static_cast<FableBitmap>(LoadImageA(
        instance,
        IntegerResource(kBootCoastalResource),
        kImageBitmap,
        0,
        0,
        kLoadCreatedDibSection));
    g_BootCoastalSunbeamArtwork =
        static_cast<FableBitmap>(LoadImageA(
            instance,
            IntegerResource(kBootCoastalSunbeamResource),
            kImageBitmap,
            0,
            0,
            kLoadCreatedDibSection));
    if (
        g_BootMenuArtwork != 0 &&
        g_BootCoastalArtwork != 0 &&
        g_BootCoastalSunbeamArtwork != 0)
    {
        GetObjectA(
            g_BootMenuArtwork,
            sizeof(g_BootMenuArtworkInfo),
            &g_BootMenuArtworkInfo);
        if (g_BootBuffJesusMenuArtwork != 0)
        {
            GetObjectA(
                g_BootBuffJesusMenuArtwork,
                sizeof(g_BootBuffJesusMenuArtworkInfo),
                &g_BootBuffJesusMenuArtworkInfo);
        }
        GetObjectA(
            g_BootCoastalArtwork,
            sizeof(g_BootCoastalArtworkInfo),
            &g_BootCoastalArtworkInfo);
        GetObjectA(
            g_BootCoastalSunbeamArtwork,
            sizeof(g_BootCoastalSunbeamArtworkInfo),
            &g_BootCoastalSunbeamArtworkInfo);
    }
    else
    {
        DeleteObject(g_BootCoastalSunbeamArtwork);
        DeleteObject(g_BootCoastalArtwork);
        DeleteObject(g_BootBuffJesusMenuArtwork);
        DeleteObject(g_BootMenuArtwork);
        g_BootCoastalSunbeamArtwork = 0;
        g_BootCoastalArtwork = 0;
        g_BootBuffJesusMenuArtwork = 0;
        g_BootMenuArtwork = 0;
    }
#if defined(FABLETLC_RETAIL_FRONTEND_SUBSCREENS)
    g_BootOptionsArtwork = static_cast<FableBitmap>(LoadImageA(
        instance,
        IntegerResource(kBootOptionsResource),
        kImageBitmap,
        0,
        0,
        kLoadCreatedDibSection));
    g_BootHelpersArtwork = static_cast<FableBitmap>(LoadImageA(
        instance,
        IntegerResource(kBootHelpersResource),
        kImageBitmap,
        0,
        0,
        kLoadCreatedDibSection));
    if (
        g_BootOptionsArtwork != 0 &&
        g_BootHelpersArtwork != 0)
    {
        GetObjectA(
            g_BootOptionsArtwork,
            sizeof(g_BootOptionsArtworkInfo),
            &g_BootOptionsArtworkInfo);
        GetObjectA(
            g_BootHelpersArtwork,
            sizeof(g_BootHelpersArtworkInfo),
            &g_BootHelpersArtworkInfo);
    }
    else
    {
        DeleteObject(g_BootHelpersArtwork);
        DeleteObject(g_BootOptionsArtwork);
        g_BootHelpersArtwork = 0;
        g_BootOptionsArtwork = 0;
    }
#endif
#endif

    FableBitmapInfo emptyAnimationArtwork = {};
    const bool useFrontendAnimation =
        commandLine == 0 ||
        strstr(commandLine, "--retail-frontend-static") == 0;
    const FableBitmapInfo& forestArtworkInfo =
        useFrontendAnimation
            ? g_BootForestArtworkInfo
            : emptyAnimationArtwork;
    const FableBitmapInfo& sunbeamArtworkInfo =
        useFrontendAnimation
            ? g_BootSunbeamArtworkInfo
            : emptyAnimationArtwork;
    const bool useBuffJesusMenu =
        commandLine != 0 &&
        strstr(commandLine, "--buff-jesus") != 0 &&
        g_BootBuffJesusMenuArtwork != 0;
    const FableBitmapInfo& menuArtworkInfo =
        useBuffJesusMenu
            ? g_BootBuffJesusMenuArtworkInfo
            : g_BootMenuArtworkInfo;
    if (FableInitialiseVisualD3D9(
            window,
            1280,
            720,
            g_BootArtworkInfo.width,
            g_BootArtworkInfo.height,
            g_BootArtworkInfo.widthBytes,
            g_BootArtworkInfo.bitsPerPixel,
            g_BootArtworkInfo.pixels,
            g_BootTitleArtworkInfo.width,
            g_BootTitleArtworkInfo.height,
            g_BootTitleArtworkInfo.widthBytes,
            g_BootTitleArtworkInfo.bitsPerPixel,
            g_BootTitleArtworkInfo.pixels,
            forestArtworkInfo.width,
            forestArtworkInfo.height,
            forestArtworkInfo.widthBytes,
            forestArtworkInfo.bitsPerPixel,
            forestArtworkInfo.pixels,
            sunbeamArtworkInfo.width,
            sunbeamArtworkInfo.height,
            sunbeamArtworkInfo.widthBytes,
            sunbeamArtworkInfo.bitsPerPixel,
            sunbeamArtworkInfo.pixels,
            menuArtworkInfo.width,
            menuArtworkInfo.height,
            menuArtworkInfo.widthBytes,
            menuArtworkInfo.bitsPerPixel,
            menuArtworkInfo.pixels,
            g_BootCoastalArtworkInfo.width,
            g_BootCoastalArtworkInfo.height,
            g_BootCoastalArtworkInfo.widthBytes,
            g_BootCoastalArtworkInfo.bitsPerPixel,
            g_BootCoastalArtworkInfo.pixels,
            g_BootCoastalSunbeamArtworkInfo.width,
            g_BootCoastalSunbeamArtworkInfo.height,
            g_BootCoastalSunbeamArtworkInfo.widthBytes,
            g_BootCoastalSunbeamArtworkInfo.bitsPerPixel,
            g_BootCoastalSunbeamArtworkInfo.pixels,
            g_BootOptionsArtworkInfo.width,
            g_BootOptionsArtworkInfo.height,
            g_BootOptionsArtworkInfo.widthBytes,
            g_BootOptionsArtworkInfo.bitsPerPixel,
            g_BootOptionsArtworkInfo.pixels,
            g_BootHelpersArtworkInfo.width,
            g_BootHelpersArtworkInfo.height,
            g_BootHelpersArtworkInfo.widthBytes,
            g_BootHelpersArtworkInfo.bitsPerPixel,
            g_BootHelpersArtworkInfo.pixels))
    {
#if defined(FABLETLC_RETAIL_FRONTEND_ARTWORK)
        SetWindowTextA(
            window,
            g_RetailProgressDisplayActive
                ? kD3DRetailAssetActiveWindowTitle
                : kD3DRetailAssetReadyWindowTitle);
#else
        SetWindowTextA(
            window,
            g_RetailProgressDisplayActive
                ? kD3DCheckpointActiveWindowTitle
                : kD3DCheckpointReadyWindowTitle);
#endif
    }

    ShowWindow(window, showCommand == 0 ? kShowNormal : showCommand);
    UpdateWindow(window);
#if defined(FABLETLC_RETAIL_FRONTEND_ANIMATION)
    SetTimer(window, kFrontendAnimationTimer, 50, 0);
#endif
    if (FableWasVisualD3D9Presented())
    {
#if defined(FABLETLC_RETAIL_FRONTEND_ARTWORK)
        if (useBuffJesusMenu)
        {
            SetWindowTextA(
                window,
                kD3DBuffJesusPresentedWindowTitle);
        }
        else
        {
            SetWindowTextA(
                window,
                g_RetailProgressDisplayActive
                    ? kD3DRetailAssetPresentedActiveWindowTitle
                    : kD3DRetailAssetPresentedReadyWindowTitle);
        }
#else
        SetWindowTextA(
            window,
            g_RetailProgressDisplayActive
                ? kD3DCheckpointPresentedActiveWindowTitle
                : kD3DCheckpointPresentedReadyWindowTitle);
#endif
    }

    if (retailVideoRequested)
    {
        g_RetailVideoWindow = window;
        g_RetailVideoInstance = instance;
        g_RetailBootMovieIndex = 0;
        g_RetailBootSequenceActive = false;
        g_RetailVideoReachedRunningState = false;
        g_RetailVideoEscapePressed = false;
        FableSetRetailVideoPreferUpscaled(
            strstr(commandLine, "--retail-video-upscaled") != 0 &&
            strstr(commandLine, "--retail-video-original") == 0);

        const char* selectedMovie = 0;
        if (strstr(commandLine, "--retail-video=intro") != 0)
            selectedMovie = "intro_comp.wmv";
        else if (
            strstr(
                commandLine,
                "--retail-video=attract") != 0)
        {
            selectedMovie = "fable_attract_english.wmv";
        }
        else if (
            strstr(
                commandLine,
                "--retail-video=lionhead") != 0)
        {
            selectedMovie = "lionhead_logo.wmv";
        }
        else if (
            strstr(
                commandLine,
                "--retail-video=microsoft") != 0)
        {
            selectedMovie = "microsoft_logo.wmv";
        }
        else
        {
            g_RetailBootSequenceActive = true;
            selectedMovie =
                kRetailBootMovies[g_RetailBootMovieIndex];
        }

        if (FableStartRetailVideo(
                window,
                instance,
                selectedMovie))
        {
            SetWindowTextA(window, kRetailVideoStartingWindowTitle);
            SetTimer(
                window,
                kRetailVideoTimer,
                50,
                0);
        }
        else
        {
            SetWindowTextA(
                window,
                FableGetRetailVideoStatus());
            RevealVisualFrontend(window);
        }
    }

    FableMessage message = {};
    while (GetMessageA(&message, 0, 0, 0) > 0)
    {
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }

    FableShutdownRetailVideo();
    FableShutdownVisualD3D9();
    UnregisterClassA(kWindowClassName, instance);
#if defined(FABLETLC_RETAIL_FRONTEND_MENU)
#if defined(FABLETLC_RETAIL_FRONTEND_SUBSCREENS)
    DeleteObject(g_BootHelpersArtwork);
    DeleteObject(g_BootOptionsArtwork);
    g_BootHelpersArtwork = 0;
    g_BootOptionsArtwork = 0;
#endif
    DeleteObject(g_BootCoastalSunbeamArtwork);
    DeleteObject(g_BootCoastalArtwork);
    DeleteObject(g_BootBuffJesusMenuArtwork);
    DeleteObject(g_BootMenuArtwork);
    g_BootCoastalSunbeamArtwork = 0;
    g_BootCoastalArtwork = 0;
    g_BootBuffJesusMenuArtwork = 0;
    g_BootMenuArtwork = 0;
#endif
    DeleteObject(g_BootArtwork);
    g_BootArtwork = 0;
#if defined(FABLETLC_RETAIL_FRONTEND_ARTWORK)
    DeleteObject(g_BootTitleArtwork);
    g_BootTitleArtwork = 0;
#if defined(FABLETLC_RETAIL_FRONTEND_ANIMATION)
    DeleteObject(g_BootSunbeamArtwork);
    DeleteObject(g_BootForestArtwork);
    g_BootSunbeamArtwork = 0;
    g_BootForestArtwork = 0;
#endif
#endif
    return static_cast<long>(message.wordParameter);
}
