#pragma once

#include "rebuild_abi.h"

struct HINSTANCE__;
typedef HINSTANCE__* FableInstanceHandle;

// Globals written by the retail WinMain wrapper before it transfers control
// to GFMain. Their names retain the retail virtual addresses until their
// engine-owned identities are recovered.
extern fable_u8* g_FableMicroThreadStackTop_013B7C84;
extern fable_u8* g_FableMicroThreadStackBottom_013B7C88;

extern "C" int FABLE_STDCALL WinMain(
    FableInstanceHandle instance,
    FableInstanceHandle previousInstance,
    char* commandLine,
    int showCommand);

#pragma pack(push, 1)
struct GFInitialiseState
{
    fable_u8 unknown00[0x08];
    fable_u32 startupValue08;
    fable_u8 unknown0C[0x200];
    fable_u8 startupFlag20C;
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(GFInitialiseState) == 0x20D);

void* FABLE_CDECL FableOperatorNew(fable_u32 size);
void FABLE_CDECL FableOperatorDelete(void* allocation);

struct CProgressDisplay
{
    fable_u8 unknown00[0x88];

    CProgressDisplay();

    __forceinline static void* operator new(unsigned int size)
    {
        return FableOperatorNew(size);
    }
};

FABLE_STATIC_ASSERT(sizeof(CProgressDisplay) == 0x88);

typedef void (FABLE_FASTCALL *FableDestroyReferencedObject)(void* object);

struct FableReferenceCount
{
    fable_i32 owners;
    FableDestroyReferencedObject destroy;
    void* object;
};

struct CCountedProgressDisplay
{
    CProgressDisplay* object;
    FableReferenceCount* reference;

    CCountedProgressDisplay(CProgressDisplay* value);

    __forceinline ~CCountedProgressDisplay()
    {
        if (reference != 0)
        {
            --reference->owners;
            if (reference->owners == 0)
            {
                reference->destroy(reference->object);
                FableOperatorDelete(reference);
            }
        }
    }
};

fable_u8 FABLE_FASTCALL GFInitialiseState_Begin(GFInitialiseState* state);
void FABLE_FASTCALL SetProgressDisplay(CCountedProgressDisplay* display);

fable_u8 FABLE_FASTCALL GFInitialise_SetupProgressDisplay(
    GFInitialiseState* state);

bool FABLE_FASTCALL GFInitialise();

// Authored engine boundary used to run the verified retail coordinator before
// the surrounding renderer and singleton graph have been recovered.
void FABLE_FASTCALL FablePrepareGFInitialiseBoundary();

struct GFInitialiseProgressPhaseState
{
    GFInitialiseState progressDisplay;
    fable_i32 displayWidth;
    fable_i32 halfDisplayWidth;
};

// Authored integration unit for the proven tail of retail GFInitialise
// 0x004023BE-0x004023E7. It calls the retail-matched progress-display setup
// leaf and carries the successful display-width result forward. It is not
// claimed as a standalone retail function.
fable_u8 FABLE_FASTCALL FableRunGFInitialiseProgressPhase(
    GFInitialiseProgressPhaseState& state);

extern GFInitialiseProgressPhaseState
    g_GFInitialiseProgressPhaseState;

// Observable Stage 4 boundary state. These counters are reconstruction
// fixture instrumentation and are not retail globals.
extern bool g_FableProgressBeginEnabled;
extern fable_u32 g_FableProgressBeginCalls;
extern fable_u32 g_FableProgressConstructCalls;
extern fable_u32 g_FableProgressInstallCalls;
extern fable_u32 g_FableProgressDestroyCalls;
