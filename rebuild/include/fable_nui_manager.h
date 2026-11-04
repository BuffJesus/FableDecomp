#pragma once

// NUISystem::CManager / NUISystem::CFrontEndManager as the frontend game
// component sees them. Names and signatures are FableWin.pdb's; offsets are
// retail's (Ego_r/FableWin put m_bHasErrorMessage at +0xC8/+0xE4, retail at
// +0x9C: CNewFrontendGameComponent::Update 0x0042DC94 inlines
// HasErrorMessage as `cmp byte ptr [eax+0x9c],0`). Retail CManager is 0xD0
// bytes (CManager::GetInstance 0x0041E5F2 allocates 0xD0); only the members
// the reconstructed callers touch are modelled.

#include "fable_2d_vector.h"
#include "rebuild_abi.h"
#include "fable_string.h"
#include "fable_stlport_deque.h"
#include "fable_stlport_list.h"
#include "fable_stlport_tree.h"
#include "fable_date_and_time.h"
// FableWin.pdb enum; values remain to be recovered.
enum FRONTEND_SCREEN { FRONTEND_SCREEN_UNKNOWN = 0 };

class CNewFrontendGameComponent;
class CVirtualKeyboard;
class CGraphicDataBank;
class CMeshDataBank;
class CInputManager;
class CDefinitionManager;
class CIEngine;
class CEnginePrimitiveHandle;
template<typename T> class CCountedPointer;

// FableWin.pdb enum ERRORMESSAGE_TYPE (global scope).
enum ERRORMESSAGE_TYPE
{
    EMT_CONTROLLER_DISCONNECT = 0,
    EMT_DIRTY_DISC = 1
};

namespace NUISystem
{
    // FableWin.pdb enum NUISystem::EEvent.
    enum EEvent
    {
        UI_EVENT_UP = 0,
        UI_EVENT_DOWN = 1,
        UI_EVENT_LEFT = 2,
        UI_EVENT_RIGHT = 3,
        UI_EVENT_SELECT = 4,
        UI_EVENT_BACK = 5,
        UI_EVENT_RIGHT_STICK_UP = 6,
        UI_EVENT_RIGHT_STICK_DOWN = 7,
        UI_EVENT_KEYBOARD_ARROWLEFT = 8,
        UI_EVENT_KEYBOARD_ARROWRIGHT = 9,
        UI_EVENT_KEYBOARD_SPACE = 10,
        UI_EVENT_KEYBOARD_BACKSPACE = 11,
        UI_EVENT_ZOOM_IN = 12,
        UI_EVENT_ZOOM_OUT = 13,
        UI_EVENT_SPELL_ROTATE_LEFT = 14,
        UI_EVENT_SPELL_ROTATE_RIGHT = 15,
        UI_EVENT_SPELL_LIST_UP = 16,
        UI_EVENT_SPELL_LIST_DOWN = 17,
        UI_EVENT_SPELL_ASSIGN = 18,
        UI_EVENT_SPELL_UNASSIGN = 19,
        UI_EVENT_SLIDER_LEFT = 20,
        UI_EVENT_SLIDER_RIGHT = 21,
        UI_EVENT_DELETE = 22,
        UI_EVENT_X = 23,
        UI_EVENT_Y = 24,
        UI_EVENT_MOUSE_MOVEMENT = 25,
        UI_EVENT_LEFT_MOUSE_CLICK = 26,
        UI_EVENT_RIGHT_MOUSE_CLICK = 27,
        UI_EVENT_LEFT_MOUSE_RELEASE = 28,
        UI_EVENT_RIGHT_MOUSE_RELEASE = 29,
        UI_EVENT_COMPONENT_DROPPED = 30,
        UI_EVENT_LEFT_MOUSE_HELD = 31,
        UI_EVENT_RIGHT_MOUSE_HELD = 32,
        UI_EVENT_KEY_DOWN = 33,
        UI_EVENT_CHAR_DOWN = 34,
        UI_EVENT_MIDDLE_MOUSE_CLICK = 35,
        UI_EVENT_MOUSE_WHEEL_UP = 36,
        UI_EVENT_MOUSE_WHEEL_DOWN = 37,
        UI_EVENT_MOUSE_BUTTON_4_CLICK = 38,
        UI_EVENT_MOUSE_BUTTON_5_CLICK = 39,
        UI_EVENT_MOUSE_BUTTON_6_CLICK = 40,
        UI_EVENT_MOUSE_BUTTON_7_CLICK = 41,
        UI_EVENT_MOUSE_BUTTON_8_CLICK = 42
    };

    class CObserver;

    // Retail layout 0x10: vptr, Observers (STLport list, 4 bytes) +0x04,
    // m_pExclusiveObserver +0x08, ConcurrentExclusiveObservers +0x0C.
    // Virtual order is retail CManager vtable 0x01230134 slots 0-10 (bodies
    // 0x0055CB10..0x0055CAE0 matched to the FableWin.pdb CObservable names by
    // arity and the list/field they touch).
    class CObservable
    {
    public:
        virtual void SetEvent(EEvent event);                                 // slot 0  0x0055CB10
        virtual void SetOwnedEvent(CObserver* observer, EEvent event);       // slot 1  0x0055CBE0
        virtual void AddObserver(CObserver* observer);                       // slot 2  0x0055CA40
        virtual void AddExclusiveObserver(CObserver* observer);              // slot 3  0x0055C930
        virtual void RemoveExclusiveObserver(CObserver* observer);           // slot 4  0x0055C940
        virtual void RemoveObserver(CObserver* observer);                    // slot 5  0x0055C9C0
        virtual void ClearObservers();                                       // slot 6  0x0055C950
        virtual CObserver* GetExclusiveObserver();                           // slot 7  0x0042BEA9
        virtual bool IsExclusivelyObserved();                                // slot 8  0x0055CAE0
        virtual void AddConcurrentExclusiveObserver(CObserver* observer);    // slot 9  0x0055CA90
        virtual void RemoveConcurrentExclusiveObserver(CObserver* observer); // slot 10 0x0055CA00

    private:
        fable_u8 Observers_[0x04];                    // list<CObserver*>
        CObserver* m_pExclusiveObserver;              // +0x08
        fable_u8 ConcurrentExclusiveObservers_[0x04]; // list<CObserver*>
    };
}

namespace NUISystem
{
    class CComponent;

    class CManager : public CObservable
    {
    public:
        // Retail vtable 0x01230134 slots 11-13.
        virtual bool GetMouseDisabled();            // slot 11 0x0042D4F0 (reads +0xC9)
        virtual void SetKeyPressed(wchar_t key);    // slot 12 0x0042D4F7 (writes +0xC0)
        virtual wchar_t GetKeyPressed();            // slot 13 0x0042D506

        // Retail 0x0041E5F2: ?GetInstance@CManager@NUISystem@@SIPAV12@XZ.
        static CManager* FABLE_FASTCALL GetInstance();

        // Inline in retail (FableWin ?HasErrorMessage@CManager@NUISystem@@QAE_NXZ).
        bool HasErrorMessage()
        {
            return m_bHasErrorMessage;
        }

        // Retail 0x0041CE6A.
        void ClearErrorMessage();

        // Retail 0x0041DB84: ?ShowErrorMessage@CManager@NUISystem@@QAEXW4ERRORMESSAGE_TYPE@@@Z.
        void ShowErrorMessage(ERRORMESSAGE_TYPE type);

        // Retail 0x0042FB40: ?Clear@CManager@NUISystem@@SIXXZ (callers
        // CNewFrontendGameComponent::Init2 0x0042E98F, ~CFrontEndManager).
        static void FABLE_FASTCALL Clear();

        // Retail 0x0041D03C: ?RenderErrorMessage@CManager@NUISystem@@QAEXAAVCIEngine@@@Z
        // (caller CNewFrontendGameComponent::Draw 0x0042DF9E).
        void RenderErrorMessage(CIEngine& engine);

    protected:
        // Retail 0x0041E3F6: ??0CManager@NUISystem@@IAE@XZ (GetInstance's
        // lazily constructed 0xD0-byte singleton).
        CManager();

        // ?GPUniqueInstance@CManager@NUISystem@@1PAV12@A, retail 0x013B8710
        // (FableWin.pdb data symbol at FableWin 0x04ABD7AC).
        static CManager* GPUniqueInstance;

    private:
        fable_u8 unknown10_[0x9C - 0x10];   // after the CObservable base

    protected:
        bool m_bHasErrorMessage;            // +0x9C

    public:
        // Ego_r.pdb members from m_pErrorMessage on, at retail offsets
        // (Ego_r +0x30: SetKeyPressed/GetMouseDisabled bodies above).
        CComponent* m_pErrorMessage;        // +0xA0
        bool Restoring;                     // +0xA4
        double m_fLastRenderTime;           // +0xA8
        C2DVector MouseMovement;            // +0xB0
        CComponent* PMouseCursor;           // +0xB8
        CComponent* PDropped;               // +0xBC
        wchar_t KeyPressed;                 // +0xC0
        float DescriptionScrollingOffset;   // +0xC4
        bool DescriptionScrollingToMax;     // +0xC8
        bool MouseDisabled;                 // +0xC9
        bool AssignmentItemsExpressionsSwapping; // +0xCA
        bool AssignmentSpellsSwapping;      // +0xCB
    };

    // RTTI (vtable 0x012521A8): CFrontEndManager : CObserver -- not
    // CManager (that singleton is a separate 0xD0 object, CManager :
    // CObservable, vtable 0x01230134). Slot 0 is the scalar deleting
    // dtor 0x0059B641.
    class CObserver
    {
    public:
        CObserver();
        virtual ~CObserver();

    private:
        _STL::set<EEvent> EventsToObserve;    // +0x04 STLport set<EEvent>
        bool PreventObservation;            // +0x10
    };

    class CActionParam;
    class CFrontEndManager : public CObserver
    {
    public:
        // Ego_r.pdb name; retail +0x14. Run 0x0042EC7C stores the movie
        // player's error text here when intro playback fails.
        CWideString MediaPlayerErrorString;

        // FableWin ??1CFrontEndManager@NUISystem@@UAE@XZ.
        virtual ~CFrontEndManager();

        // Retail 0x00595A03: ?CanStartAttractMode@CFrontEndManager@NUISystem@@QAE_NXZ.
        bool CanStartAttractMode();
        // Retail 0x00599E3F: ?Update@CFrontEndManager@NUISystem@@QAEXM@Z.
        void Update(float timePassed);
        // Retail 0x00595582: ?GetInstance@CFrontEndManager@NUISystem@@SIPAV12@XZ
        // (lazily allocates the 0xE0-byte manager).
        static CFrontEndManager* FABLE_FASTCALL GetInstance();
        // Retail 0x005952C3: ?ChangeStateFirstTime@CFrontEndManager@NUISystem@@QAEXXZ.
        void ChangeStateFirstTime();

        // Retail 0x005953E2: ??0CFrontEndManager@NUISystem@@QAE@XZ.
        CFrontEndManager();

    protected:
        // ?m_pInstance@CFrontEndManager@NUISystem@@1PAV12@A, retail 0x013B8B5C
        // (FableWin.pdb data symbol at FableWin 0x04ABEB54).
        static CFrontEndManager* m_pInstance;

    public:
        // Retail offsets from the ctor 0x005953E2 (list ctor at +0x18, stack
        // ctor at +0x20, vector at +0x48) and Init2 0x0042E98F (writes the
        // owning component to +0x1C). Ego_r.pdb names.
        _STL::list<CActionParam*> UIActionParams;        // +0x18
        CNewFrontendGameComponent* PGameComponent;      // +0x1C
        _STL::stack<CComponent*> m_stackScreens;        // +0x20
        _STL::vector<CComponent*> m_vComponents;        // +0x48
        _STL::map<FRONTEND_SCREEN,CComponent*> m_mapScreens; // +0x54
        CVirtualKeyboard* m_pVirtualKeyboard;          // +0x60
        bool m_bStartNewGame;                          // +0x64
        _STL::vector<CDateAndTime> m_vFileTimes;         // +0x68
        _STL::vector<CCharString> FileRegionNames;      // +0x74
        _STL::vector<CCharString> FileRegionMiniMapNames;// +0x80
        _STL::vector<long> FilePlayTimes;               // +0x8C
        CComponent* PScreenToDeactivate;               // +0x98
        CComponent* PScreenToActivate;                 // +0x9C
        bool GoToKeyboard;                             // +0xA0
        CComponent* PScreenToDeactivateForStartGame;    // +0xA4
        CComponent* PScreenToDeactivateForLoadGame;     // +0xA8
        CWideString GameToLoad;                        // +0xAC
        CWideString ProfileToDelete;                   // +0xB0
        unsigned long BlocksOnUNeeded, BlocksOnTNeeded, NumberSaveGames;
        bool m_bQuitFromGame;                          // +0xC0
        _STL::map<CWideString,bool> Signatures;          // +0xC4
        double InstallationPauseStartTime;             // +0xD0
        bool InstallationPaused, FinalInstallationNeverPause; // +0xD8

        // FableWin ?SetGameComponent@CFrontEndManager@NUISystem@@QAEXPAVCNewFrontendGameComponent@@@Z;
        // inline in retail (Init2 0x0042E98F: `mov [eax+0x1c], esi`).
        void SetGameComponent(CNewFrontendGameComponent* component)
        {
            PGameComponent = component;
        }

        // FableWin ?SetQuitFromGame@CFrontEndManager@NUISystem@@QAEX_N@Z;
        // inline in retail (Init2 0x0042E98F writes +0xC0).
        void SetQuitFromGame(bool quitFromGame)
        {
            m_bQuitFromGame = quitFromGame;
        }

        // Retail 0x005958F5: ?Init@CFrontEndManager@NUISystem@@QAEXV?$CCountedPointer@VCGraphicDataBank@@@@V?$CCountedPointer@VCMeshDataBank@@@@PAVCInputManager@@PAVCDefinitionManager@@@Z
        // (both counted pointers by value).
        void Init(CCountedPointer<CGraphicDataBank> graphicBank,
                  CCountedPointer<CMeshDataBank> meshBank,
                  CInputManager* inputManager,
                  CDefinitionManager* definitionManager);
        // Retail 0x00598A1C: ?BuildComponents@CFrontEndManager@NUISystem@@QAEX_N@Z.
        void BuildComponents(bool flag);
        // Retail 0x00595222: ?Draw@CFrontEndManager@NUISystem@@QAEXAAVCIEngine@@PAVCEnginePrimitiveHandle@@@Z.
        void Draw(CIEngine& engine, CEnginePrimitiveHandle* handle);
    };
}

FABLE_STATIC_ASSERT(sizeof(NUISystem::CFrontEndManager) == 0xE0);

// Retail 0x0041E61B: ?ShowErrorMessage@@YIXW4ERRORMESSAGE_TYPE@@@Z.
void FABLE_FASTCALL ShowErrorMessage(ERRORMESSAGE_TYPE type);

namespace NUISystem
{
    class CUIScreenUnitConverter
    {
    public:
        // Retail 0x0040F0E0: ?ResetDimensions@CUIScreenUnitConverter@NUISystem@@SIXXZ.
        static void FABLE_FASTCALL ResetDimensions();

    private:
        // ?BackBufferWidth / ?BackBufferHeight@CUIScreenUnitConverter@NUISystem@@0MA,
        // retail 0x013B876C / 0x013B8770 (FableWin.pdb data at 0x04ABD744 / 0x04ABD748).
        static float BackBufferWidth;
        static float BackBufferHeight;
    };
}

FABLE_STATIC_ASSERT(sizeof(NUISystem::CObservable) == 0x10);
FABLE_STATIC_ASSERT(sizeof(NUISystem::CManager) == 0xD0);
