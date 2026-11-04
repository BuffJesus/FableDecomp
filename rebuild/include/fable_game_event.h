#pragma once

// CGameEvent with its inline members (FableWin ??0CGameEvent@@QAE@J@Z,
// ?AddUBYTE@CGameEvent@@QAEXE@Z). Layout is Ego_r.pdb's (0x28), confirmed by
// retail CMainGameComponent::Update 0x00418289, which inlines the constructor
// (Type, Player=-1, EndPos=0, Valid=true, Replacement=false) and AddUBYTE
// (a call to AddData(1, &value), retail 0x009F1760).
//
// Do not include together with the generated flat "engine/CGameEvent.h".

#include "rebuild_abi.h"

// Ego_r.pdb EGameEventType (only the values reconstructed code names).
enum EGameEventType
{
    GAME_EVENT_UPDATE_FRAME = 1,
    // CMainGameComponent::ProcessEvent 0x00415FE0 / EventIsSystemEvent
    // 0x00415FF2 test for it (FableWin.pdb value 2).
    GAME_EVENT_QUIT = 2
};

// Ego_r.pdb CGameEventDataPackage (0x2C): a read cursor over an event's data.
class CGameEventDataPackage
{
public:
    // FableWin ??0CGameEventDataPackage@@QAE@XZ; inline in retail
    // (CMainGameComponent::CheckSync 0x004165E8 zeroes the three longs).
    CGameEventDataPackage()
        : Length(0),
          StartPos(0),
          EndPos(0)
    {
    }

    // FableWin ?GetData@CGameEventDataPackage@@QAEXJPAX@Z (retail 0x009F18F0).
    void GetData(long size, void* data);

    // FableWin ?GetULONG@CGameEventDataPackage@@QAEKXZ; inline in retail.
    unsigned char GetUBYTE()
    {
        unsigned char value;
        GetData(1, &value);
        return value;
    }

    // FableWin GetSLONG; original signed four-byte read, inlined in retail.
    long GetSLONG()
    {
        long value;
        GetData(4, &value);
        return value;
    }

    unsigned long GetULONG()
    {
        unsigned long value;
        GetData(4, &value);
        return value;
    }

    unsigned char Data[32];     // +0x00
    long Length;                // +0x20
    long StartPos;              // +0x24
    long EndPos;                // +0x28
};

FABLE_STATIC_ASSERT(sizeof(CGameEventDataPackage) == 0x2C);

class CGameEvent
{
public:
    explicit CGameEvent(long type)
        : Type(type),
          Player(-1),
          EndPos(0),
          Valid(true),
          Replacement(false)
    {
    }

    // FableWin ??0CGameEvent@@QAE@XZ; inline in retail
    // (CMainGameComponent::GetInputs 0x00416E78 stores only Player=-1 and
    // Valid=false: `mov byte [ebp-0x28],0xff` / `mov byte [ebp-6],0`).
    CGameEvent()
        : Player(-1),
          Valid(false)
    {
    }

    // FableWin ?AddData@CGameEvent@@QAEXJPBX@Z, retail 0x009F1760.
    void AddData(long size, const void* data);

    void AddUBYTE(unsigned char value)
    {
        AddData(1, &value);
    }

    // FableWin ?AddULONG@CGameEvent@@QAEXK@Z; inline in retail (AddData(4, &value):
    // CNetworkClient::GetLocalGameEventPackageSet 0x004AEAA0).
    void AddULONG(unsigned long value)
    {
        AddData(4, &value);
    }

    // FableWin ?Initialise@CGameEvent@@QAEXJ@Z and ?SetPlayer@CGameEvent@@QAEXJ@Z;
    // inline in retail (0x004AEAA0 stores Type, EndPos=0, Valid=1, Replacement=0,
    // Player=-1).
    void Initialise(long type)
    {
        Type = type;
        Player = -1;
        EndPos = 0;
        Valid = true;
        Replacement = false;
    }

    void SetPlayer(long player)
    {
        Player = static_cast<signed char>(player);
    }

    // FableWin ?GetType@CGameEvent@@QBEJXZ; inline in retail.
    long GetType() const
    {
        return Type;
    }

    // FableWin ?GetDataPackage@CGameEvent@@QBEXPAVCGameEventDataPackage@@@Z
    // (retail 0x009F17A0).
    void GetDataPackage(CGameEventDataPackage* package) const;

    long Type;                  // +0x00
    signed char Player;         // +0x04
    unsigned char Data[32];     // +0x05
    unsigned char EndPos;       // +0x25
    bool Valid;                 // +0x26
    bool Replacement;           // +0x27
};

FABLE_STATIC_ASSERT(sizeof(CGameEvent) == 0x28);

// FableWin CGameEventPackage: only the out-of-line readers are modelled
// (?GetNoEvents@CGameEventPackage@@QBEKXZ retail 0x009F16E0,
// ?GetEvent@CGameEventPackage@@QBEPBVCGameEvent@@K@Z retail 0x009F16D0).
class CGameEventPackage
{
public:
    CGameEventPackage() : NoEvents(0) {}
    unsigned long GetNoEvents() const;
    const CGameEvent* GetEvent(unsigned long index) const;

    // FableWin ?AddEvent@CGameEventPackage@@QAEXPBVCGameEvent@@@Z (retail
    // 0x009F1650) and ?Clear@CGameEventPackage@@QAEXXZ (retail 0x009F16C0);
    // ?SetTimeStamp@CGameEventPackage@@QAEXJ@Z is inline in retail.
    void AddEvent(const CGameEvent* event);
    void Clear();
    void SetTimeStamp(long timeStamp)
    {
        TimeStamp = timeStamp;
    }

    // FableWin ?GetTimeStamp@CGameEventPackage@@QBEJXZ; inline in retail
    // (CMainGameComponent::UpdateFromEventPackageSet 0x0041726D spills its
    // by-value result to a temporary before Maximum()).
    long GetTimeStamp() const
    {
        return TimeStamp;
    }

    // Ego_r.pdb layout (0x648). CMainGameComponent::UpdateFromEventPackageSet
    // 0x0041726D reads TimeStamp (+0x00) directly.
    long TimeStamp;                 // +0x00
    unsigned long NoEvents;         // +0x04
    CGameEvent Events[40];          // +0x08
};

FABLE_STATIC_ASSERT(sizeof(CGameEventPackage) == 0x648);
