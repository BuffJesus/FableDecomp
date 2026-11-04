#pragma once

#include "rebuild_abi.h"
#include "fable_string.h"

struct HINSTANCE__;
typedef HINSTANCE__* FableInstanceHandle;
struct CDataBank;

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

// The retail CGame singleton (0x013B83D0); fable_game.h has the class.
class CGame;

void* FABLE_CDECL FableOperatorNew(fable_u32 size);
void FABLE_CDECL FableOperatorDelete(void* allocation);

// Retail std::vector is three pointers (0xC bytes); its default ctor nulls them.
template <class T>
struct FableVector
{
    T* first;
    T* last;
    T* end;

    FableVector() : first(0), last(0), end(0)
    {
    }
};

class CTexture;

// Retail RTTI .?AVCProgressDisplay@@: a class.
class CProgressDisplay
{
public:
    void* vftable00;
    FableVector<CTexture> ProgressTextures;
    FableVector<CTexture> GuildSealTextures;
    FableVector<CTexture> LoadingScreenTextures;
    fable_i32 LoadingScreenIndex;
    FableVector<CCharString> TextTags;
    FableVector<CCharString> TextTagsPC;
    fable_u32 value44;
    CWideString wideText48;
    CCharString primaryText4C;
    CCharString secondaryText50;
    fable_u32 value54;
    fable_u32 value58;
    fable_u32 unknown5C;
    double progressTime60;
    double displayTime68;
    double completionTime70;
    fable_u8 flag78;
    bool active79;
    fable_u8 drawRetail7A;
    fable_u8 flag7B;
    fable_u8 displayText7C;
    fable_u8 flag7D;
    fable_u8 flag7E;
    fable_u8 flag7F;
    fable_u8 flag80;
    fable_u8 flag81;
    fable_u8 unknown82[6];

    CProgressDisplay();
    bool IsActive() const;
    void SetToDisplayText(bool enabled);
    void CalculateNextTextTag();
    CDataBank* GetPTextBank() const;

    __forceinline static void* operator new(size_t size)
    {
        return FableOperatorNew(size);
    }
};

FABLE_STATIC_ASSERT(sizeof(CProgressDisplay) == 0x88);

#pragma pack(push, 1)
struct FableProgressTextBankSlot
{
    fable_u8 unknown00[0x14];
    CDataBank* textBank14;
};

struct FableGameTextBankSlot
{
    fable_u8 unknown00[0x60];
    CDataBank* textBank60;
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(FableProgressTextBankSlot) == 0x18);
FABLE_STATIC_ASSERT(sizeof(FableGameTextBankSlot) == 0x64);

struct FableReferenceCount;

extern fable_u8 g_CProgressDisplayVTable_012388B4;
extern double g_FableZeroDouble_0122ED70;
extern const char g_FableEmptyText_0122D70E[];
extern FableProgressTextBankSlot*
    g_FableProgressTextBankSlot_013B86A0;
extern FableGameTextBankSlot*
    g_FableGameTextBankSlot_013B871C;
void FABLE_FASTCALL FableConstructWideString(CWideString* value);
void FABLE_FASTCALL FableConstructCharString(CCharString* value);

typedef void (FABLE_FASTCALL *FableDestroyReferencedObject)(void* object);

struct FableReferenceCount
{
    fable_i32 owners;
    FableDestroyReferencedObject destroy;
    void* object;
};

void FABLE_FASTCALL FableDestroyProgressDisplay_00403600(void* object);

struct CCountedProgressDisplay
{
    CProgressDisplay* object;
    FableReferenceCount* reference;

    __forceinline CCountedProgressDisplay() : object(0), reference(0)
    {
    }

    CCountedProgressDisplay(CProgressDisplay* value);

    __forceinline CCountedProgressDisplay(
        const CCountedProgressDisplay& other)
        : object(other.object),
          reference(other.reference)
    {
        if (reference != 0)
            ++reference->owners;
    }

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

// Legacy boot ABI view of NProgressDisplay::PProgressDisplay at013CAA38.
// Both views are two pointers; this linker alias keeps one actual CRT owner.
// The installed concrete display has CAProgressDisplay at offset zero.
extern CCountedProgressDisplay g_FableProgressDisplay_013CAA38;
#pragma comment(linker,"/alternatename:?g_FableProgressDisplay_013CAA38@@3UCCountedProgressDisplay@@A=?PProgressDisplay@NProgressDisplay@@3V?$CCountedPointer@VCAProgressDisplay@@@@A")

void FABLE_FASTCALL SetProgressDisplay(CCountedProgressDisplay* display);
CCountedProgressDisplay FABLE_FASTCALL GetProgressDisplay();
void FABLE_FASTCALL FableReleaseProgressDisplayBoundary();

bool FABLE_FASTCALL GFInitialise();

// Authored engine boundary used to run the verified retail coordinator before
// the surrounding renderer and singleton graph have been recovered.
void FABLE_FASTCALL FablePrepareGFInitialiseBoundary();

struct GFInitialiseProgressPhaseState
{
    CGame* game;
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
