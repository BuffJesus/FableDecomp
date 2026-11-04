#pragma once

// Navigator classes with RETAIL layouts (navigation TUs 0x00A14E20..0x00A1A150).
// Names and signatures: Ego_r.pdb / FableWin.pdb. Ancestry: retail RTTI class
// hierarchy descriptors:
//   CNavigatorManager : CClassFactory : CBaseClassNonCopyable : CBaseClass  (vtable 0x0129CA84)
//   CNavigatorBase    : CClass : CBaseObject : CBaseClass                    (vtable 0x0129D4A4)
//   CNavigatorFlyer   : CNavigatorBase                                       (vtable 0x0129CB54)
//   CNavigatorAStar   : CNavigatorBase                                       (vtable 0x0129CCCC)
//
// Retail containers are STLport (list = 4 bytes, vector = 12, map = 12). Ego_r.pdb was
// built against the MS STL, whose containers are larger, so the generated POD views
// engine/CNavigatorManager.h (QueueManager +0x40) and the Ego_r CNavigatorAStar
// (LastNavigableNodeVisited +0x90) drift after the first container. Retail evidence:
//   ??_GCNavigatorManager 0x00A175A0 destroys QueueManager (+0x2C) then QuadTrees (+0x24);
//   five callers add 0x2C to NavigationManager before RemoveNavigatorFromQueue 0x00A155C0;
//   CNavigatorAStar ctor 0x00A1CA70 stores States +0x24, the three blocked nodes
//   +0x30/+0x40/+0x50, OpenList +0x60..+0x68, LastNavigableNodeVisited +0x88, Route +0x8C,
//   LastKnownNavPos +0xA8, LastKnownNavSpeed +0xB4, LastKnownNavLayer +0xB8, Initialised +0xBD.
//
// Do not include together with the generated POD views engine/CNavigatorBase.h,
// engine/CNavigatorFlyer.h or engine/CNavigatorManager.h (same class names).

#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_2d_vector.h"
#include "fable_class.h"
#include "fable_counted_pointer.h"
#include "fable_display_geometry.h"
#include "fable_nav_waypoint.h"    // CNavWayPoint (Route element)
#include "fable_stlport_list.h"
#include "fable_stlport_tree.h"   // _STL::pair
#include "fable_stlport_vector.h"
#include "fable_string.h"
#include "fable_thing_manager.h"  // CClassFactory

class CANavQuadTreeNode;
class CClassTypeInit;
class CNavigationAreaInit;
class CNavigatorBase;
class CNavigatorExternalInterface;
class CNavigatorManagerExternalInterface;
class CNavQuadTree;
class CSteeringAgent;

// Ego_r enum ENavigatorReturn (enumerators not needed by these roots).
enum ENavigatorReturn
{
};

// Ego_r vector<pair<C2DVector, unsigned char> > (pre-calculated route).
typedef _STL::vector<_STL::pair<C2DVector, unsigned char> > CNavigatorRoute;

// Ego_r CIsNavigator (4 bytes): list predicate matching one navigator (FableWin
// ??0CIsNavigator@@QAE@ABVCNavigatorBase@@@Z, ??RCIsNavigator@@QBE_NABU?$pair@JPBVCNavigatorBase@@@std@@@Z;
// both inline in retail list::remove_if<CIsNavigator> 0x00A16EB0).
class CIsNavigator
{
public:
    CIsNavigator(const CNavigatorBase& navigator) : Navigator(&navigator) {}
    bool operator()(const _STL::pair<long, const CNavigatorBase*>& entry) const
    {
        return entry.second == Navigator;
    }

    const CNavigatorBase* Navigator; // +0x00
};

// Ego_r CNavQuadTreeMap (32 bytes): retail list<CNavQuadTreeMap>::clear 0x00A17390
// releases PQuadTree (node+0x08) and SectionName (node+0x24).
class CNavQuadTreeMap
{
public:
    CNavQuadTreeMap(); // retail 0x00A16D50
    // Destructor implicit: retail clear 0x00A17390 expands it in place.

    CCountedPointer<CNavQuadTree> PQuadTree; // +0x00
    long Index;                              // +0x08
    C2DBoxI MapArea;                         // +0x0C
    CCharString SectionName;                 // +0x1C
};

// Ego_r CNavigatorQueueManager: one list, Navigators.
class CNavigatorQueueManager
{
public:
    // Ego_r ?RemoveNavigatorFromQueue@CNavigatorQueueManager@@QAEXABVCNavigatorBase@@@Z,
    // retail 0x00A155C0 (tail call to list::remove_if<CIsNavigator> 0x00A16EB0).
    void RemoveNavigatorFromQueue(const CNavigatorBase& navigator);

    _STL::list<_STL::pair<long, const CNavigatorBase*> > Navigators; // +0x00
};

// Retail vtable 0x0129CA84: ??_G 0x00A175A0, AddNewClassType 0x00A17590,
// OnCreateClass 0x0051DA20, OnDeleteClass 0x0051DA30 (CClassFactory slots 1..3).
class CNavigatorManager : public CClassFactory
{
public:
    virtual ~CNavigatorManager();
    virtual void AddNewClassType(const CClassTypeInit& init); // 0x00A17590

    // Ego_r ?FrameUpdate@CNavigatorManager@@QAEXXZ, retail 0x00A14E20.
    void FrameUpdate();
    // Ego_r ?UpdateNavigationMapArea@CNavigatorManager@@QAEXJABVCNavigationAreaInit@@@Z (retail 0x00A14E30)
    // and ?UpdateNavigationMapArea@CNavigatorManager@@QAEXJABVC2DBoxF@@@Z (retail 0x00A14E40);
    // both compiled empty in retail (FableWin's editor build has bodies).
    void UpdateNavigationMapArea(long index, const CNavigationAreaInit& area);
    void UpdateNavigationMapArea(long index, const C2DBoxF& area);
    // FableWin ?GetNavigatorQueueManager@CNavigatorManager@@QAEAAVCNavigatorQueueManager@@XZ;
    // inline in retail (CNavigatorAStar::Interrupt 0x00A1A150 adds 0x2C).
    CNavigatorQueueManager& GetNavigatorQueueManager() { return QueueManager; }

    unsigned long* Seed;                                   // +0x1C
    long NumberOfNodeExaminationsLeftThisFrame;            // +0x20
    _STL::list<CNavQuadTreeMap> QuadTrees;                 // +0x24
    CNavigatorManagerExternalInterface* PInterface;        // +0x28
    CNavigatorQueueManager QueueManager;                   // +0x2C
};

// Retail vtable 0x0129D4A4 (20 slots; 3..15 are _purecall). Slot 1 is
// CBaseObject::Validate, slot 2 CClass::Save.
class CNavigatorBase : public CClass
{
public:
    // Ego_r ??0CNavigatorBase@@QAE@AAVCNavigatorManager@@JABVCNavigatorExternalInterface@@@Z,
    // retail 0x00A47110; destructor 0x00A47180.
    CNavigatorBase(CNavigatorManager& manager, unsigned long type, const CNavigatorExternalInterface& external);
    virtual ~CNavigatorBase();
    virtual ENavigatorReturn MoveToPos(const C3DVector& from, const C3DVector& to, float speed,
                                       C3DVector& movement, bool a, bool b, bool c, bool d,
                                       bool e, bool f, float g) = 0;                         // 3
    virtual ENavigatorReturn MoveToPosProximity(const C3DVector& from, const C3DVector& to,
                                                float proximity, float speed, C3DVector& movement,
                                                bool a, bool b, bool c, bool d, bool e, bool f,
                                                float g) = 0;                                // 4
    virtual ENavigatorReturn MoveToNavLineOfSightProximity(const C3DVector& from, const C3DVector& to,
                                                           float proximity, float speed,
                                                           C3DVector& movement, bool a, bool b,
                                                           bool c, bool d, bool e, bool f,
                                                           float g) = 0;                     // 5
    // VC7.1 places overloaded virtuals in reverse declaration order (slots 7, 6).
    virtual C3DVector GetRandomNavigablePos(const C3DVector& pos, float radius, bool flag) const = 0;
    virtual C3DVector GetRandomNavigablePos(const C3DVector& pos, float radius,
                                            const C2DVector& dir, float angle) const = 0;
    virtual bool GetNearestNavigablePosTo(const C3DVector& pos, float radius,
                                          C3DVector& out) const = 0;                        // 8
    virtual bool CanNavigateToPos(const C3DVector& from, const C3DVector& to, float radius,
                                  bool flag) const = 0;                                      // 9
    virtual bool IsNavLineOfSightClear(const C3DVector& from, const C3DVector& to) const = 0;  // 10
    virtual bool IsNewDestinationGoingToCauseRestart(const C3DVector& dest, float d) const = 0; // 11
    virtual const C3DVector& GetCurrentDestination() const = 0;                             // 12
    virtual void UpdateLastPosition(const C3DVector& pos) = 0;                              // 13
    virtual void CalculateRouteFromTo(const C3DVector& from, const C3DVector& to,
                                      CNavigatorRoute& route) = 0;                           // 14
    virtual void InitialiseFromPreCalculatedRoute(const C3DVector& from, const CNavigatorRoute& route,
                                                  const C3DVector& to) = 0;                  // 15
    virtual const CANavQuadTreeNode* GetNextNeighbour(const CANavQuadTreeNode& node,
                                                      const CANavQuadTreeNode* previous,
                                                      const C2DVector& dir);                 // 16 (0x00A472F0)
    // Slots 17..19 are inline in the original header: retail keeps their only copies
    // (0x00A19AC0, 0x00A19AD0, 0x00A19AE0) as COMDATs inside the navigator-flyer TU,
    // away from the CNavigatorBase TU (0x00A47110..), and CNavigatorAStar::Interrupt
    // 0x00A1A150 expands CNavigatorBase::Interrupt in place.
    virtual void CreateLineList(_STL::vector<C3DVector>&) {}                                // 17
    virtual void Interrupt() { Interrupted = true; }                                         // 18
    virtual void CalculateNavigableWidthOfLine(const C3DVector&, const C3DVector&,
                                               float& left, float& right)                    // 19
    {
        right = 0.0f;
        left = 0.0f;
    }

    CNavigatorExternalInterface* PInterface; // +0x14
    bool OnRoute;                            // +0x18
    bool Interrupted;                        // +0x19
    CNavigatorManager* NavigationManager;    // +0x1C
    CSteeringAgent* SteeringAgent;           // +0x20
};

// Retail vtable 0x0129CB54; constructor 0x00A18F40 (navigator type 4).
class CNavigatorFlyer : public CNavigatorBase
{
protected:
    // Ego_r ??0CNavigatorFlyer@@IAE@AAVCNavigatorManager@@ABVCNavigatorExternalInterface@@@Z.
    CNavigatorFlyer(CNavigatorManager& manager, const CNavigatorExternalInterface& external);
public:
    virtual ENavigatorReturn MoveToPos(const C3DVector& from, const C3DVector& to, float speed,
                                       C3DVector& movement, bool a, bool b, bool c, bool d,
                                       bool e, bool f, float g);
    virtual ENavigatorReturn MoveToPosProximity(const C3DVector& from, const C3DVector& to,
                                                float proximity, float speed, C3DVector& movement,
                                                bool a, bool b, bool c, bool d, bool e, bool f,
                                                float g);
    virtual ENavigatorReturn MoveToNavLineOfSightProximity(const C3DVector& from, const C3DVector& to,
                                                           float proximity, float speed,
                                                           C3DVector& movement, bool a, bool b,
                                                           bool c, bool d, bool e, bool f,
                                                           float g);
    virtual C3DVector GetRandomNavigablePos(const C3DVector& pos, float radius, bool flag) const;
    virtual C3DVector GetRandomNavigablePos(const C3DVector& pos, float radius,
                                            const C2DVector& dir, float angle) const;
    virtual bool GetNearestNavigablePosTo(const C3DVector& pos, float radius, C3DVector& out) const;
    virtual bool CanNavigateToPos(const C3DVector& from, const C3DVector& to, float radius,
                                  bool flag) const;
    virtual bool IsNavLineOfSightClear(const C3DVector& from, const C3DVector& to) const;
    virtual bool IsNewDestinationGoingToCauseRestart(const C3DVector& dest, float d) const;
    // Inline (COMDATs 0x00A19B00 / 0x00A19B10 follow the CNavigatorBase ones).
    virtual const C3DVector& GetCurrentDestination() const { return DestinationPos; }
    virtual void UpdateLastPosition(const C3DVector&) {}
    virtual void CalculateRouteFromTo(const C3DVector& from, const C3DVector& to,
                                      CNavigatorRoute& route);
    virtual void InitialiseFromPreCalculatedRoute(const C3DVector& from, const CNavigatorRoute& route,
                                                  const C3DVector& to);

    C3DVector DestinationPos; // +0x24
};

// Retail vtable 0x0129CCCC (slot 0 ??_G 0x00A224D0); constructor 0x00A1CA70
// (navigator type 3). Container members stay opaque where no retail-layout
// STLport type exists in rebuild/include.
class CNavigatorAStar : public CNavigatorBase
{
public:
    virtual ~CNavigatorAStar();
    virtual ENavigatorReturn MoveToPos(const C3DVector& from, const C3DVector& to, float speed,
                                       C3DVector& movement, bool a, bool b, bool c, bool d,
                                       bool e, bool f, float g);
    virtual ENavigatorReturn MoveToPosProximity(const C3DVector& from, const C3DVector& to,
                                                float proximity, float speed, C3DVector& movement,
                                                bool a, bool b, bool c, bool d, bool e, bool f,
                                                float g);
    virtual ENavigatorReturn MoveToNavLineOfSightProximity(const C3DVector& from, const C3DVector& to,
                                                           float proximity, float speed,
                                                           C3DVector& movement, bool a, bool b,
                                                           bool c, bool d, bool e, bool f,
                                                           float g);
    virtual C3DVector GetRandomNavigablePos(const C3DVector& pos, float radius, bool flag) const;
    virtual C3DVector GetRandomNavigablePos(const C3DVector& pos, float radius,
                                            const C2DVector& dir, float angle) const;
    virtual bool GetNearestNavigablePosTo(const C3DVector& pos, float radius, C3DVector& out) const;
    virtual bool CanNavigateToPos(const C3DVector& from, const C3DVector& to, float radius,
                                  bool flag) const;
    virtual bool IsNavLineOfSightClear(const C3DVector& from, const C3DVector& to) const;
    virtual bool IsNewDestinationGoingToCauseRestart(const C3DVector& dest, float d) const;
    virtual const C3DVector& GetCurrentDestination() const;
    virtual void UpdateLastPosition(const C3DVector& pos);
    virtual void CalculateRouteFromTo(const C3DVector& from, const C3DVector& to,
                                      CNavigatorRoute& route);
    virtual void InitialiseFromPreCalculatedRoute(const C3DVector& from, const CNavigatorRoute& route,
                                                  const C3DVector& to);
    virtual void CreateLineList(_STL::vector<C3DVector>& lines);                  // 17 (0x00A1BFC0)
    virtual void Interrupt();                                                     // 18 (0x00A1A150)
    virtual void CalculateNavigableWidthOfLine(const C3DVector& from, const C3DVector& to,
                                               float& left, float& right);        // 19 (0x00A1DF60)

    unsigned char      States[0xC];                    // +0x24 map<long, CAStarState>
    unsigned char      BlockedStartNode[0x10];         // +0x30 CNavBlockedQuadTreeNode (ctor 0x00A768C0)
    unsigned char      BlockedEndNode[0x10];           // +0x40 CNavBlockedQuadTreeNode
    unsigned char      BlockedNodeAskedToGetTo[0x10];  // +0x50 CNavBlockedQuadTreeNode
    unsigned char      OpenList[0xC];                  // +0x60 CPriorityQueue<CAStarState*>
    CANavQuadTreeNode* GoalNode;                       // +0x6C
    C2DVector          GoalPosition;                   // +0x70
    CANavQuadTreeNode* NodeAskedToGetTo;               // +0x78
    C3DVector          PosAskedToGetTo;                // +0x7C
    CANavQuadTreeNode* LastNavigableNodeVisited;       // +0x88
    _STL::list<CNavWayPoint> Route;                    // +0x8C
    _STL::list<CNavWayPoint>::iterator CullFront;      // +0x90
    _STL::list<CNavWayPoint>::iterator CullBack;       // +0x94
    C2DVector          LastWayPointVisited;            // +0x98
    C2DVector          StringLeadPosition;             // +0xA0
    C3DVector          LastKnownNavPos;                // +0xA8
    float              LastKnownNavSpeed;              // +0xB4
    long               LastKnownNavLayer;              // +0xB8
    bool               HeadingForActionPoint;          // +0xBC
    bool               Initialised;                    // +0xBD
    bool               GotRoute;                       // +0xBE
    bool               SlowedDownForLastPoint;         // +0xBF
    bool               Culled;                         // +0xC0
    bool               PartWayThroughAStar;            // +0xC1
    unsigned char      NoOfClockwiseTurns;             // +0xC2
    unsigned char      NoOfAntiClockwiseTurns;         // +0xC3
};

FABLE_STATIC_ASSERT(sizeof(CIsNavigator) == 4);
FABLE_STATIC_ASSERT(sizeof(CNavQuadTreeMap) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CNavQuadTreeMap, SectionName) == 0x1C);
FABLE_STATIC_ASSERT(sizeof(CNavigatorQueueManager) == 4);
FABLE_STATIC_ASSERT(sizeof(CNavigatorManager) == 0x30);
FABLE_STATIC_ASSERT(offsetof(CNavigatorManager, NumberOfNodeExaminationsLeftThisFrame) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CNavigatorManager, QuadTrees) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CNavigatorManager, QueueManager) == 0x2C);
FABLE_STATIC_ASSERT(sizeof(CNavigatorBase) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CNavigatorBase, PInterface) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CNavigatorBase, Interrupted) == 0x19);
FABLE_STATIC_ASSERT(offsetof(CNavigatorBase, NavigationManager) == 0x1C);
FABLE_STATIC_ASSERT(sizeof(CNavigatorFlyer) == 0x30);
FABLE_STATIC_ASSERT(offsetof(CNavigatorFlyer, DestinationPos) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CNavigatorAStar, BlockedStartNode) == 0x30);
FABLE_STATIC_ASSERT(offsetof(CNavigatorAStar, OpenList) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CNavigatorAStar, LastNavigableNodeVisited) == 0x88);
FABLE_STATIC_ASSERT(offsetof(CNavigatorAStar, Route) == 0x8C);
FABLE_STATIC_ASSERT(offsetof(CNavigatorAStar, LastKnownNavPos) == 0xA8);
FABLE_STATIC_ASSERT(offsetof(CNavigatorAStar, LastKnownNavLayer) == 0xB8);
FABLE_STATIC_ASSERT(offsetof(CNavigatorAStar, Initialised) == 0xBD);
FABLE_STATIC_ASSERT(sizeof(CNavigatorAStar) == 0xC4);
