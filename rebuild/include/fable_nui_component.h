#pragma once

// NUISystem::CComponent -- the frontend UI element base class. Only its
// virtual interface is modelled (retail layout not recovered yet).
//
// Slot order is retail vtable 0x0124608C (RTTI .?AVCComponent@NUISystem@@,
// complete-object offset 0), matched slot-by-slot against FableWin.exe's
// CComponent vtable (FableWin.pdb names, same `ret N` per slot):
//   slot 0 0x00535C10 ??_E (scalar deleting dtor) ... slot 48 0x0052E710
//   ?ChangeState@CComponent@NUISystem@@UAEXK@Z (empty in the base, `ret 4`).
// VC7.1 lays a group of overloaded virtuals out in REVERSE declaration
// order, so the three ChangeState overloads are declared (K), (K,M),
// (CUIState&) to land at slots 48, 47, 46 as retail has them.
// Callers: NUISystem::CManager::ClearErrorMessage 0x0041CE6A (slot 48 and
// the deleting dtor), NUISystem::CFrontEndManager::ChangeStateFirstTime
// 0x005952C3 (slot 48).

#include "rebuild_abi.h"

class C2DVector;
class CRGBColour;
class CCharString;
class CIEngine;
class CEnginePrimitiveHandle;
class C2DBoxF;
template<typename T> class CCountedPointer;

namespace _STL
{
    template <class _Tp> class allocator;
    template <class _Tp, class _Alloc> class list;
    template <class _Tp, class _Alloc> class vector;
}

namespace NUISystem
{
    class CUIState;
    class CAction;
    class CComponent;

    typedef _STL::list<CComponent*, _STL::allocator<CComponent*> > CComponentPtrList;
    typedef _STL::vector<CCountedPointer<CComponent>,
                         _STL::allocator<CCountedPointer<CComponent> > > CComponentChildVector;

    class CComponent
    {
    public:
        // FableWin NUISystem::CComponent::EType (enumerators not recovered).
        enum EType
        {
        };

        virtual ~CComponent();                                                  // slot  0 0x00535C10
        virtual void Update(float timePassed);                                  // slot  1 0x00531EC0
        virtual void Draw(CIEngine& engine, CEnginePrimitiveHandle* handle,
                          long layer, long* count, CComponent* viewport);       // slot  2 0x00530260
        virtual CComponent* FindChild(CCharString name);                        // slot  3 0x00530780
        virtual CComponent* FindChildForward(CCharString name);                 // slot  4 0x00530830
        virtual void FindAllChildren(CCharString name, CComponentPtrList& out); // slot  5 0x00530F30
        virtual void FindAllChildrenOfType(EType type, CComponentPtrList& out); // slot  6 0x00531000
        virtual void SetPAcceptButton(CComponent* button);                      // slot  7 0x0041C590
        virtual CComponent* FindChildNonRecursive(CCharString name);            // slot  8 0x005308F0
        virtual CComponent* FindFirstChildOfType(EType type);                   // slot  9 0x00530460
        virtual bool HasChild(CComponent* child) const;                         // slot 10 0x005309E0
        virtual void SetZoom(const C2DVector& zoom);                            // slot 11 0x0052E750
        virtual C2DVector GetZoom() const;                                      // slot 12 0x0052E780
        virtual void SetColour(const CRGBColour& colour);                       // slot 13 0x0052E7A0
        virtual CRGBColour GetColour() const;                                   // slot 14 0x0052F500
        virtual void SetPosition(const C2DVector& position);                    // slot 15 0x0052E860
        virtual C2DVector GetPosition() const;                                  // slot 16 0x0052E890
        virtual C2DVector GetParentPosition() const;                            // slot 17 0x0052E8B0
        virtual void SetParentPosition(C2DVector& position);                    // slot 18 0x0052E8D0
        virtual C2DVector GetParentZoom() const;                                // slot 19 0x0052E8F0
        virtual void SetParentZoom(C2DVector& zoom);                            // slot 20 0x0052E910
        virtual CRGBColour GetParentColour() const;                             // slot 21 0x0052F580
        virtual void SetParentColour(CRGBColour& colour);                       // slot 22 0x0052E930
        virtual C2DVector GetSize() const;                                      // slot 23 0x00533D30
        virtual C2DBoxF GetBoundingBox() const;                                 // slot 24 0x00531090
        virtual const C2DVector& GetRenderPosition() const;                     // slot 25 0x0052E940
        virtual const C2DVector& GetRenderZoom() const;                         // slot 26 0x0052E950
        virtual const CRGBColour& GetRenderColour() const;                      // slot 27 0x0052E960
        virtual void SetRenderPosition(C2DVector& position);                    // slot 28 0x0052E970
        virtual void SetRenderZoom(C2DVector& zoom);                            // slot 29 0x0052E990
        virtual void SetRenderColour(CRGBColour& colour);                       // slot 30 0x0052E9B0
        virtual void ChangePosition(const C2DVector& position, float time, bool linear);      // slot 31 0x0052E9C0
        virtual void ChangePositionDelta(const C2DVector& delta, float time, bool linear);    // slot 32 0x0052EA30
        virtual void ChangeTargetPositionDelta(const C2DVector& delta, float time, bool linear); // slot 33 0x0052EAF0
        virtual void UpdatePosition(float timePassed);                          // slot 34 0x0052FFD0
        virtual void ChangeZoom(const C2DVector& zoom, float time, bool linear);          // slot 35 0x0052EBB0
        virtual void ChangeZoomDelta(const C2DVector& delta, float time, bool linear);    // slot 36 0x0052EC20
        virtual void UpdateZoom(float timePassed);                              // slot 37 0x0052F5C0
        virtual void ChangeColour(const CRGBColour& colour, float time, bool linear);     // slot 38 0x0052EC60
        virtual void ChangeColourDelta(const CRGBColour& delta, float time, bool linear); // slot 39 0x005305A0
        virtual void UpdateColour(float timePassed);                            // slot 40 0x0052F900
        virtual void ResetState();                                              // slot 41 0x0052E6B0
        virtual void ForceState(unsigned long state, bool immediate);           // slot 42 0x0052E700
        virtual void Initialise();                                              // slot 43 0x005339B0
        virtual bool HasState(unsigned long state);                             // slot 44 0x0052F340
        virtual bool HasStateInChildren(unsigned long state);                   // slot 45 0x00530530
        // Reverse-declared overload group -> slots 48, 47, 46.
        virtual void ChangeState(unsigned long state);                          // slot 48 0x0052E710
        virtual void ChangeState(unsigned long state, float time);              // slot 47 0x0052E720
        virtual void ChangeState(CUIState& state);                              // slot 46 0x0052E740
        virtual bool ChangedState();                                            // slot 49 0x0052E730
        virtual CUIState* GetCurrentState();                                    // slot 50 0x00533B30
        virtual void SetParent(CComponent* parent);                             // slot 51 0x0052F330
        virtual CComponent* GetParent() const;                                  // slot 52 0x0052E7C0
        virtual CComponentChildVector& GetChildren();                           // slot 53 0x0052E7D0
        virtual float GetUpdateTime() const;                                    // slot 54 0x0052E7E0
        virtual C2DVector GetTargetPosition() const;                            // slot 55 0x0052E7F0
        virtual CRGBColour GetTargetColour() const;                             // slot 56 0x0052F540
        virtual C2DVector GetTargetZoom() const;                                // slot 57 0x0052E810
        virtual void ProcessChangeState();                                      // slot 58 0x0052E840
        // Retail has 134 slots; the rest are not modelled yet.
    };
}
