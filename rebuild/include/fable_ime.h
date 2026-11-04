#pragma once

#include <stddef.h>

#include "rebuild_abi.h"
#include "fable_rgb_colour.h"
#include "fable_2d_vector.h"

struct HWND__;
struct tagMSG;
class CPersistContext;
class CCharString;

// DXUT ImeUi (built into the game with its default __fastcall convention).
class CImeUiFont_Base
{
public:
    virtual void SetHeight(unsigned int height) = 0;
    virtual void SetColor(unsigned long colour) = 0;
    virtual void SetPosition(int x, int y) = 0;
    virtual void GetTextExtent(
        const wchar_t* text,
        unsigned long* width,
        unsigned long* height) = 0;
    virtual void DrawTextW(const wchar_t* text) = 0;
};

struct IMEUI_APPEARANCE
{
    unsigned long symbolColor;
    unsigned long symbolColorOff;
    unsigned long symbolColorText;
    unsigned char symbolHeight;
    unsigned char symbolTranslucence;
    unsigned char symbolPlacement;
    CImeUiFont_Base* symbolFont;
    unsigned long candColorBase;
    unsigned long candColorBorder;
    unsigned long candColorText;
    unsigned long compColorInput;
    unsigned long compColorTargetConv;
    unsigned long compColorConverted;
    unsigned long compColorTargetNotConv;
    unsigned long compColorInputErr;
    unsigned char compTranslucence;
    unsigned long compColorText;
    unsigned char caretWidth;
    unsigned char caretYMargin;
};

struct IMEUI_VERTEX;

bool FABLE_FASTCALL ImeUi_Initialize(HWND__* window, bool disable);
void FABLE_FASTCALL ImeUi_SetAppearance(const IMEUI_APPEARANCE* appearance);
void FABLE_FASTCALL ImeUi_SetScreenDimension(
    unsigned int width,
    unsigned int height);
void FABLE_FASTCALL ImeUi_FinalizeString(bool send);
void FABLE_FASTCALL ImeUi_EnableIme(bool enable);
void FABLE_FASTCALL ImeUi_RenderUI(bool drawIndicator, bool drawComposition); // 00CB38E0

extern void (FABLE_FASTCALL* ImeUiCallback_DrawRect)(
    int x1,
    int y1,
    int x2,
    int y2,
    unsigned long colour);
extern void* (FABLE_FASTCALL* ImeUiCallback_Malloc)(unsigned int bytes);
extern void (FABLE_FASTCALL* ImeUiCallback_Free)(void* pointer);
extern void (FABLE_FASTCALL* ImeUiCallback_DrawFans)(
    const IMEUI_VERTEX* vertices,
    unsigned int count);
extern void (FABLE_FASTCALL* ImeUiCallback_OnChar)(wchar_t character);

namespace NUISystem
{
    class CEditBox;

    class CIMEFont : public CImeUiFont_Base
    {
        friend struct CIMERecoveryTestAccess;
    public:
        virtual void SetHeight(unsigned int height);
        virtual void SetColor(unsigned long colour);
        virtual void SetPosition(int x, int y);
        virtual void GetTextExtent(
            const wchar_t* text,
            unsigned long* width,
            unsigned long* height);
        virtual void DrawTextW(const wchar_t* text);

    private:
        void GetFontNameAndScale(CCharString& name, float& scale) const; // 00404970
        CRGBColour Colour;
        C2DVector Position;
        unsigned long Height;
    };

    class CIME
    {
        friend struct CIMERecoveryTestAccess;
    public:
        static void FABLE_FASTCALL LoadIMESettings(
            CPersistContext& context);
        static void FABLE_FASTCALL ResetToDefaultSettings(); // 00404A90
        // ?Get@CIME@NUISystem@@SIAAV12@XZ (retail 0x00404A80) and
        // ?Render@CIME@NUISystem@@QAEXXZ (retail 0x00404C00); caller
        // CNewFrontendGameComponent::Draw 0x0042DF9E.
        static CIME& FABLE_FASTCALL Get();
        static bool FABLE_FASTCALL ProcessWinMessage(
            HWND__* window, unsigned int message, unsigned int wParam,
            long lParam, long& result);
        static bool FABLE_FASTCALL FilterMessage(tagMSG* message);
        void Render();
        void Initialise(HWND__* window);
        void Shutdown(); // 004051C0; original Ego_r identity.
        void Enable(bool enable);
        void SetScreenDimensions(long width, long height);

    private:

        static void FABLE_FASTCALL Callback_DrawRect(
            int x1,
            int y1,
            int x2,
            int y2,
            unsigned long colour);
        static void FABLE_FASTCALL Callback_DrawFans(
            const IMEUI_VERTEX* vertices,
            unsigned int count);
        static void FABLE_FASTCALL Callback_OnChar(wchar_t character);
        static void* FABLE_FASTCALL Callback_malloc(unsigned int bytes);
        static void FABLE_FASTCALL Callback_free(void* pointer);

        bool EnableIME;
        HWND__* WindowHandle;
        bool IsInitialised;
        IMEUI_APPEARANCE Appearance;
        CEditBox* PEditBox;
        bool CancelOnOKClick;
    };
}

FABLE_STATIC_ASSERT(sizeof(IMEUI_APPEARANCE) == 0x40);
FABLE_STATIC_ASSERT(sizeof(NUISystem::CIMEFont) == 0x14);
FABLE_STATIC_ASSERT(sizeof(NUISystem::CIME) == 0x54);

extern NUISystem::CIME g_FableIME_013B7CD8;
