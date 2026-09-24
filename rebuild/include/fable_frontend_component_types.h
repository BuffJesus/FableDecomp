#pragma once

#include "rebuild_abi.h"

// Retail RTTI for CNewFrontendGameComponent (primary vtable 0x01230CA0,
// secondary 0x01230C94) proves this base hierarchy and callback offset +4.
class CBaseClass
{
public:
    CBaseClass();
    virtual ~CBaseClass();
};

class CBaseClassNonCopyable : public CBaseClass
{
protected:
    CBaseClassNonCopyable() {}
private:
    CBaseClassNonCopyable(const CBaseClassNonCopyable&);
    CBaseClassNonCopyable& operator=(const CBaseClassNonCopyable&);
};

class CDeviceResetCallback
{
public:
    virtual void OnPreDeviceReset();
    virtual bool OnPostDeviceReset();
};

// These are storage contracts, not replacements for the engine's ownership
// operations. Construction clears them; release/growth need their real helpers.
template<class T> struct FableFrontendCountedStorage
{
    T* Data;
    void* Info;
    FableFrontendCountedStorage() : Data(0), Info(0) {}
};

template<class T> struct FableFrontendVectorStorage
{
    T* Begin;
    T* End;
    T* CapacityEnd;
    FableFrontendVectorStorage() : Begin(0), End(0), CapacityEnd(0) {}
};

struct FableFrontendBoxI
{
    int Left, Top, Right, Bottom;
};

struct FableFrontendInterpolationSet
{
    float GTPredictedRenderAt, GTPredictedTimeSinceLastRenderFrame, WFInterpolate;
    FableFrontendInterpolationSet()
        : GTPredictedRenderAt(0), GTPredictedTimeSinceLastRenderFrame(0), WFInterpolate(0) {}
};

class CStopWatch
{
public:
    CStopWatch();
    __int64 GetTicks() const;
    void Start();
    void StartZero();
    void Stop();
    void Reset();
    float GetElapsedSeconds() const;
    float m_fTimerPeriod;
    __int64 m_nStartTick;
    __int64 m_nPrevElapsedTicks;
    bool m_bIsRunning;
};

FABLE_STATIC_ASSERT(sizeof(CStopWatch) == 0x20);
FABLE_STATIC_ASSERT(sizeof(FableFrontendVectorStorage<void>) == 0x0C);

class CFontBank;
class CGraphicDataBank;
class CMeshDataBank;
class CASoundBank;
class CIEngine;
class CInputManager;
class CFrontEndDef;
class CFrontEndManager;
class CTexture;
class CDateAndTime;
namespace NGameText { class CDataBank; }
