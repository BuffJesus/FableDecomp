#pragma once

#include "rebuild_abi.h"
#include "fable_ui_state.h"
#include "fable_reference_count.h"
#include <stddef.h>

struct FableUiComponentDrawView;
struct FableUiPositionChildNode;
// First word of the secondary observer interface at retail component +4.
struct FableUiObserverInterfaceView { void* Vtable; };
struct FableUiDeletionParentNode;
struct FableUiDeletion
{
    unsigned Method;
    FableUiDeletionParentNode* AssociatedParents;
};
struct CUIStateRecoveredLayout;
typedef bool (__fastcall *FableUiComponentQuery)(FableUiComponentDrawView*, void*);
typedef void (__fastcall *FableUiComponentAction)(FableUiComponentDrawView*, void*);
typedef unsigned (__fastcall *FableUiComponentTypeQuery)(FableUiComponentDrawView*, void*);
typedef void (__fastcall *FableUiComponentStateChange)(FableUiComponentDrawView*, void*, unsigned, float);
typedef void (__fastcall *FableUiComponentStateRequest)(FableUiComponentDrawView*, void*, unsigned);
typedef FableUiDeletion* (__fastcall *FableUiComponentDeletionQuery)(FableUiComponentDrawView*, void*);
typedef void (__fastcall *FableUiComponentDeletionSetter)(FableUiComponentDrawView*, void*, FableUiDeletion);
typedef bool (__fastcall *FableUiComponentHasState)(FableUiComponentDrawView*, void*, unsigned);
typedef CUIStateRecoveredLayout* (__fastcall *FableUiComponentFindState)(FableUiComponentDrawView*, void*, unsigned);
typedef void (__fastcall *FableUiComponentVectorDelta)(FableUiComponentDrawView*, void*, const FableUiStateVector2*, float, bool);
typedef void (__fastcall *FableUiComponentVectorSetter)(FableUiComponentDrawView*, void*, const FableUiStateVector2*);
typedef void (__fastcall *FableUiComponentColourSetter)(FableUiComponentDrawView*, void*, const FableUiStateColour*);
typedef void (__fastcall *FableUiComponentParentSetter)(FableUiComponentDrawView*, void*, FableUiComponentDrawView*);
typedef void (__fastcall *FableUiComponentUpdateCall)(FableUiComponentDrawView*, void*, float);
typedef void (__fastcall *FableUiComponentColourDelta)(FableUiComponentDrawView*, void*, const FableUiStateColour*, float, bool);
typedef FableUiComponentDrawView* (__fastcall *FableUiComponentParentQuery)(FableUiComponentDrawView*, void*);
typedef void (__fastcall *FableUiComponentDrawCall)(FableUiComponentDrawView*, void*,
    void* engine, void* handle, long layer, long* index, FableUiComponentDrawView* parent);

// Non-owning view of the vtable entries exercised by draw and state recovery.
// Query names describe their observed role; unused slots stay unnamed.
struct FableUiComponentDrawVtable
{
    void* Unrecovered00;
    FableUiComponentUpdateCall Update;
    FableUiComponentDrawCall Draw;
    void* Unrecovered0C[15];
    FableUiComponentVectorSetter SetParentPosition;
    void* Unrecovered4C;
    FableUiComponentVectorSetter SetParentZoom;
    void* Unrecovered54;
    FableUiComponentColourSetter SetParentColour;
    void* Unrecovered5C[8];
    FableUiComponentVectorDelta ChangePosition;
    FableUiComponentVectorDelta ChangePositionDelta;
    void* Unrecovered84;
    FableUiComponentUpdateCall UpdatePosition;
    FableUiComponentVectorDelta ChangeZoom;
    FableUiComponentVectorDelta ChangeZoomDelta;
    FableUiComponentUpdateCall UpdateZoom;
    void* Unrecovered98;
    FableUiComponentColourDelta ChangeColour;
    FableUiComponentUpdateCall UpdateColour;
    void* UnrecoveredA4[3];
    FableUiComponentHasState HasState;
    void* UnrecoveredB4[2];
    FableUiComponentStateChange ChangeState;
    FableUiComponentStateRequest RequestState;
    FableUiComponentQuery HasCompletedStateChange;
    void* UnrecoveredC8;
    FableUiComponentParentSetter SetParent;
    FableUiComponentParentQuery GetParent;
    void* UnrecoveredD4[5];
    FableUiComponentAction ProcessChangeState;
    void* UnrecoveredEC[3];
    FableUiComponentDeletionSetter SetDeletion;
    FableUiComponentDeletionQuery GetDeletion;
    FableUiComponentStateRequest RemoveChildAt;
    FableUiComponentTypeQuery GetType;
    void* Unrecovered108[15];
    FableUiComponentAction Die;
    void* Unrecovered148[4];
    FableUiComponentTypeQuery GetCurrentState;
    void* Unrecovered15C[13];
    FableUiComponentQuery AcceptForeignParent;
    FableUiComponentQuery IsIndependent;
    FableUiComponentQuery IsPositionIndependent;
    FableUiComponentQuery IsZoomIndependent;
    FableUiComponentQuery IsLayerIndependent;
    FableUiComponentQuery IsDrawSuppressed;
    void* Unrecovered1A8[8];
    FableUiComponentVectorSetter SetRelativeParentPosition;
    FableUiComponentVectorSetter SetRelativeParentZoom;
    FableUiComponentQuery UseRelativeZoom;
    FableUiComponentQuery UseRelativePosition;
    void* Unrecovered1D8[8];
    FableUiComponentParentQuery GetPositionParent;
    void* Unrecovered1FC[6];
    FableUiComponentAction ObserverRemoved;
    FableUiComponentFindState FindStateForQuery;
    FableUiComponentFindState FindState;
    FableUiComponentAction UpdateStateChange;
    FableUiComponentQuery ChangedStateLastUpdate;
    FableUiComponentQuery InternalChanged;
    FableUiComponentQuery ChildrenChanged;
    FableUiComponentAction RequestedStateOneOrFive;
    FableUiComponentAction RequestedStateZeroOrSix;
};

struct FableUiComponentCountedStorage
{
    FableUiComponentDrawView* Data;
    FableReferenceCount* Info;
};

struct FableUiComponentChildrenStorage
{
    FableUiComponentCountedStorage* Begin;
    FableUiComponentCountedStorage* End;
    FableUiComponentCountedStorage* CapacityEnd;

    unsigned Size() const { return static_cast<unsigned>(End - Begin); }
};

struct FableUiShapeChildStorage
{
    unsigned* Begin;
    unsigned* End;
    unsigned* CapacityEnd;
    unsigned Size() const { return static_cast<unsigned>(End - Begin); }
};

// Retail layout view, NOT the donor CComponent (whose STL storage is larger).
// Explicitly unrecovered ranges avoid inventing ownership or member identities.
struct FableUiComponentDrawView
{
    FableUiComponentDrawVtable* Vtable;
    FableUiObserverInterfaceView Observer;
    unsigned char UnrecoveredBase[0x28];
    float Time;
    FableUiStateVector2 Position, TargetPosition, InitialPosition, ParentPosition, RenderPosition;
    FableUiStateVector2 Zoom, TargetZoom, InitialZoom, ParentZoom, RenderZoom;
    FableUiStateColour Colour, TargetColour, InitialColour, ParentColour, RenderColour;
    float PositionTimeElapsed, PositionTime, ZoomTimeElapsed, ZoomTime, ColourTimeElapsed, ColourTime;
    FableUiComponentChildrenStorage Children;
    FableUiComponentChildrenStorage ChildrenToDelete;
    FableUiComponentDrawView* Parent;
    unsigned char UnrecoveredCC[8];
    FableUiDeletion Deletion;
    unsigned char UnrecoveredDC[8];
    FableUiShapeChildStorage ShapeChildren;
    unsigned char UnrecoveredF0[8];
    FableUiStateVector2 RelativeRenderPosition, RelativeParentPosition;
    FableUiStateVector2 RelativeRenderZoom, RelativeParentZoom;
    FableUiComponentDrawView* PositionParent;
    FableUiPositionChildNode* PositionChildren;
    unsigned char Unrecovered120[0x0C];
    unsigned char Flags;
    unsigned char Type;
    unsigned char FlagsSetTwo;
    signed char Layer;
};

FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, Draw) == 0x08);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, Update) == 0x04);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, UpdatePosition) == 0x88);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, UpdateZoom) == 0x94);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, UpdateColour) == 0xA0);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawView, Time) == 0x30);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, RequestState) == 0xC0);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, GetCurrentState) == 0x158);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawView, Deletion) == 0xD4);
FABLE_STATIC_ASSERT(sizeof(FableUiDeletion) == 8);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, SetDeletion) == 0xF8);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, GetDeletion) == 0xFC);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, RemoveChildAt) == 0x100);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, Die) == 0x144);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, ObserverRemoved) == 0x214);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawView, Observer) == 4);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawView, ShapeChildren) == 0xE4);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, SetParentColour) == 0x58);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, SetParent) == 0xCC);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, GetPositionParent) == 0x1F8);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, SetParentPosition) == 0x48);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, SetParentZoom) == 0x50);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, SetRelativeParentPosition) == 0x1C8);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, SetRelativeParentZoom) == 0x1CC);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawView, PositionParent) == 0x118);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawView, PositionChildren) == 0x11C);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, HasCompletedStateChange) == 0xC4);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, ChangeState) == 0xBC);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, HasState) == 0xB0);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, ChangePosition) == 0x7C);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, ChangePositionDelta) == 0x80);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, ChangeZoom) == 0x8C);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, ChangeZoomDelta) == 0x90);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, ChangeColour) == 0x9C);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, FindState) == 0x21C);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, FindStateForQuery) == 0x218);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, ProcessChangeState) == 0xE8);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, GetType) == 0x104);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, UpdateStateChange) == 0x220);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, InternalChanged) == 0x228);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, ChildrenChanged) == 0x22C);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, GetParent) == 0xD0);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, AcceptForeignParent) == 0x190);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, IsIndependent) == 0x194);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, IsPositionIndependent) == 0x198);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, IsZoomIndependent) == 0x19C);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, UseRelativeZoom) == 0x1D0);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, UseRelativePosition) == 0x1D4);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, IsLayerIndependent) == 0x1A0);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawVtable, IsDrawSuppressed) == 0x1A4);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawView, Children) == 0xB0);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawView, Position) == 0x34);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawView, Zoom) == 0x5C);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawView, Colour) == 0x84);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawView, PositionTimeElapsed) == 0x98);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawView, ChildrenToDelete) == 0xBC);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawView, Parent) == 0xC8);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawView, Layer) == 0x12F);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawView, RelativeRenderPosition) == 0xF8);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawView, RelativeParentPosition) == 0x100);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawView, RelativeRenderZoom) == 0x108);
FABLE_STATIC_ASSERT(offsetof(FableUiComponentDrawView, RelativeParentZoom) == 0x110);

void __fastcall FableUiComponentDraw(FableUiComponentDrawView* component, void*,
    void* engine, void* handle, long parentLayer, long* index, FableUiComponentDrawView* parent);
