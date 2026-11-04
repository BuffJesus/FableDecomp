#pragma once

// CSB: abstract steering behaviour (base of CSBGoal, CSBAgents, CSBCAgents,
// CSBIAgents, CSBCCorridor). Retail RTTI .?AVCSB@@ has no base class.
// Retail vtable 0x0129CC1C: [??_G 0x00A19D00, _purecall, _purecall,
// CreateLineList 0x00A19CE0, Reset 0x00A19CF0]; destructor 0x00A19CC0.
// Slot names: FableWin.pdb CSBGoal vtable (Process, GetType). Data: Ego_r.pdb
// sizeof 0xC (vptr + CKinematic Output).
//
// Derived behaviours (hand-owned here, so gen_class_headers skips them). Ancestry:
// retail RTTI class hierarchy descriptors; vtables read back through their complete
// object locators; members: Ego_r.pdb (DIA: reference/pointer kinds, constness and
// access - data members protected/private exactly as Ego_r, member functions public).
//   CSBGoal       : CSB                              vtable 0x0129CC34
//   CSBInput      : CSB                              vtable 0x0129CC4C
//   CSBConstraint : CSBInput : CSB                   vtable 0x0129CC64
//   CSBCAgents    : CSBConstraint : CSBInput : CSB   vtable 0x0129CC7C
//   CSBIAgents    : CSBInput : CSB                   vtable 0x0129CC94 (adds slot 5 Display)
//   CSBCCorridor  : CSBConstraint : CSBInput : CSB   vtable 0x0129CCB0
// Retail containers are STLport (list = 4 bytes). Ego_r.pdb used the MS STL (list = 0xC),
// so CSBCCorridor members after Walls sit 8 bytes lower in retail than in Ego_r.
// Ego_r non-virtual members without a recovered retail body (CSB::GetOutput,
// ShouldIgnoreAgent, GetDistanceBetweenAgents, CSBInput::SetInput,
// CSBConstraint::GetMoveBeforeOrientation, CSBCCorridor::AddWall) are not declared yet.
// The derived destructors are implicit: ??_G 0x00A22330 expands them in place.
//
// Layout asserts on protected/private members live in CSteeringBehaviourLayoutCheck
// (a friend of each class; it has no members and emits no code).
//
// Do not include together with the generated POD views engine/CSB.h, engine/CSBGoal.h,
// engine/CSBIAgents.h, engine/CSBCAgents.h or engine/CSBCCorridor.h (same class names);
// those files are now include shims of this header.

#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_2d_vector.h"
#include "fable_kinematic.h"
#include "fable_stlport_list.h"
#include "fable_stlport_vector.h"

class CSteeringAgent;
struct CSteeringBehaviourLayoutCheck;

class CSB
{
public:
    // Ego_r enum CSB::ESB_TYPE (enumerator values from Ego_r.pdb).
    enum ESB_TYPE
    {
        ESB_AGENTS = 0,
        ESB_GOAL = 1,
        ESBI_AGENTS = 2,
        ESBC_CORRIDOR = 3,
        ESBC_AGENTS = 4
    };

    virtual ~CSB();                                                // 0 (??1 0x00A19CC0, ??_G 0x00A19D00)
    virtual bool Process(const CSteeringAgent* agent) = 0;         // 1
    virtual ESB_TYPE GetType() = 0;                                // 2
    virtual void CreateLineList(_STL::vector<C3DVector>& lines);   // 3 (0x00A19CE0)
    virtual void Reset();                                          // 4 (0x00A19CF0)

protected:
    friend struct CSteeringBehaviourLayoutCheck;
    CKinematic Output;          // +0x04
};

// Ego_r CWall (0x18): one wall of CSBCCorridor::Walls. Retail list<CWall> nodes are
// 0x20 bytes (list ctors 0x00A21230 / 0x00A21830). Ego_r also has inline constructors
// CWall() and CWall(C2DVector, C2DVector, C2DVector); not modelled (bodies unrecovered).
class CWall
{
public:
    C2DVector Point1;     // +0x00
    C2DVector Point2;     // +0x08
    C2DVector ToInternal; // +0x10
};

// Retail vtable 0x0129CC4C: [??_G 0x00A208A0, _purecall, _purecall, 0x00A19CE0, 0x00A19CF0].
class CSBInput : public CSB
{
protected:
    friend struct CSteeringBehaviourLayoutCheck;
    CKinematic Input;           // +0x0C
};

// Retail vtable 0x0129CC64: [??_G 0x00A208D0, _purecall, _purecall, 0x00A19CE0, 0x00A19CF0].
class CSBConstraint : public CSBInput
{
protected:
    friend struct CSteeringBehaviourLayoutCheck;
    bool MoveBeforeOrientation; // +0x14
};

// Retail vtable 0x0129CC34: [??_G 0x00A20870, Process 0x00A87580, GetType 0x00A20860,
// 0x00A19CE0, 0x00A19CF0].
class CSBGoal : public CSB
{
public:
    // Ego_r/FableWin ??0CSBGoal@@QAE@PBVC2DVector@@0PBM111@Z.
    CSBGoal(const C2DVector* goal, const C2DVector* nextGoal, const float* bestDistLeft,
            const float* bestDistRight, const float* bestNextDistLeft, const float* bestNextDistRight);
    virtual bool Process(const CSteeringAgent* agent);             // 1 (0x00A87580)
    virtual ESB_TYPE GetType();                                    // 2 (0x00A20860)

private:
    friend struct CSteeringBehaviourLayoutCheck;
    const C2DVector* Goal;              // +0x0C
    const C2DVector* NextGoal;          // +0x10
    const float* BestDistLeft;          // +0x14
    const float* BestDistRight;         // +0x18
    const float* BestNextDistLeft;      // +0x1C
    const float* BestNextDistRight;     // +0x20
};

// Retail vtable 0x0129CC7C: [??_G 0x00A20920, Process 0x00A87700, GetType 0x00A20910,
// 0x00A19CE0, 0x00A19CF0].
class CSBCAgents : public CSBConstraint
{
public:
    // Ego_r/FableWin ??0CSBCAgents@@QAE@ABV?$vector@PBVCSteeringAgent@@...@Z.
    CSBCAgents(const _STL::vector<const CSteeringAgent*>& agents);
    virtual bool Process(const CSteeringAgent* agent);             // 1 (0x00A87700)
    virtual ESB_TYPE GetType();                                    // 2 (0x00A20910)

private:
    const _STL::vector<const CSteeringAgent*>& Agents;  // +0x18
};

// Retail vtable 0x0129CC94: [??_G 0x00A211E0, Process 0x00A87960, GetType 0x00A211D0,
// 0x00A19CE0, 0x00A19CF0, Display 0x00A211C0].
class CSBIAgents : public CSBInput
{
public:
    // Ego_r/FableWin ??0CSBIAgents@@QAE@ABV?$vector@PBVCSteeringAgent@@...@Z.
    CSBIAgents(const _STL::vector<const CSteeringAgent*>& agents);
    virtual bool Process(const CSteeringAgent* agent);             // 1 (0x00A87960)
    virtual ESB_TYPE GetType();                                    // 2 (0x00A211D0)
    virtual void Display(const CSteeringAgent* agent);             // 5 (0x00A211C0)

private:
    const _STL::vector<const CSteeringAgent*>& Agents;  // +0x14
};

// Retail vtable 0x0129CCB0: [??_G 0x00A22330, Process 0x00A880C0, GetType 0x00A21F90,
// CreateLineList 0x00A887B0, Reset 0x00A88080]. ??_G 0x00A22330 destroys Walls at +0x18
// and expands the destructors in place (the destructor stays implicit here).
class CSBCCorridor : public CSBConstraint
{
public:
    // Ego_r/FableWin ??0CSBCCorridor@@QAE@XZ; retail out of line at 0x00A21F50 (body unlanded).
    CSBCCorridor();
    virtual bool Process(const CSteeringAgent* agent);             // 1 (0x00A880C0)
    virtual ESB_TYPE GetType();                                    // 2 (0x00A21F90)
    virtual void CreateLineList(_STL::vector<C3DVector>& lines);   // 3 (0x00A887B0)
    virtual void Reset();                                          // 4 (0x00A88080)

private:
    friend struct CSteeringBehaviourLayoutCheck;
    _STL::list<CWall> Walls;    // +0x18 (Reset 0x00A88080 / ??_G 0x00A22330)
    C2DVector CurrentEndPoint;  // +0x1C (Ego_r +0x24, MS-STL list drift)
    float BestLeft;             // +0x24 (Ego_r +0x2C)
    float BestRight;            // +0x28 (Ego_r +0x30)
};

struct CSteeringBehaviourLayoutCheck
{
    FABLE_STATIC_ASSERT(offsetof(CSB, Output) == 0x4);
    FABLE_STATIC_ASSERT(offsetof(CSBInput, Input) == 0xC);
    FABLE_STATIC_ASSERT(offsetof(CSBConstraint, MoveBeforeOrientation) == 0x14);
    FABLE_STATIC_ASSERT(offsetof(CSBGoal, Goal) == 0xC);
    FABLE_STATIC_ASSERT(offsetof(CSBGoal, BestNextDistRight) == 0x20);
    FABLE_STATIC_ASSERT(offsetof(CSBCCorridor, Walls) == 0x18);
    FABLE_STATIC_ASSERT(offsetof(CSBCCorridor, CurrentEndPoint) == 0x1C);
    FABLE_STATIC_ASSERT(offsetof(CSBCCorridor, BestRight) == 0x28);
};

FABLE_STATIC_ASSERT(sizeof(CSB) == 0xC);
FABLE_STATIC_ASSERT(sizeof(CWall) == 0x18);
FABLE_STATIC_ASSERT(sizeof(_STL::_List_node<CWall>) == 0x20);  // retail node (ctors 0x00A21230 / 0x00A21830)
FABLE_STATIC_ASSERT(sizeof(CSBInput) == 0x14);
FABLE_STATIC_ASSERT(sizeof(CSBConstraint) == 0x18);
FABLE_STATIC_ASSERT(sizeof(CSBGoal) == 0x24);
FABLE_STATIC_ASSERT(sizeof(CSBCAgents) == 0x1C);
FABLE_STATIC_ASSERT(sizeof(CSBIAgents) == 0x18);
// Retail allocates CSBCCorridor with operator new(0x2C) at 0x00A1C8B5 before its ctor 0x00A21F50
// (which inits Walls +0x18 and CurrentEndPoint +0x1C/+0x20).
FABLE_STATIC_ASSERT(sizeof(CSBCCorridor) == 0x2C);
// Agents are reference members (offsetof cannot name them): CSBCAgents +0x18, CSBIAgents +0x14
// follow from the base sizes 0x18 / 0x14 and the class sizes above.
