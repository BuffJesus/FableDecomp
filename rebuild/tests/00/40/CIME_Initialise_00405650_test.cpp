#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fable_gfuninitialise.h"
#include "fable_ime.h"
#include "fable_surface.h"

namespace
{
    unsigned char g_systemStorage[sizeof(CSystemManager)];
    CDisplayManager* const kDisplay =
        reinterpret_cast<CDisplayManager*>(0x44445555);

    bool g_initialiseResult;
    fable_u32 g_sequence;
    fable_u32 g_resetOrder;
    fable_u32 g_initialiseCalls;
    bool g_initialiseDisable;
    HWND__* g_initialiseWindow;
    fable_u32 g_allocations;
    fable_u32 g_allocationSize;
    fable_u32 g_appearanceCalls;
    const IMEUI_APPEARANCE* g_appearance;
    fable_u32 g_dimensionCalls;
    const CDisplayManager* g_dimensionDisplay;
    fable_u32 g_screenCalls;
    unsigned int g_screenWidth;
    unsigned int g_screenHeight;
    fable_u32 g_finaliseCalls;
    fable_u32 g_finaliseOrder;
    bool g_finaliseSend;
    fable_u32 g_enableCalls;
    fable_u32 g_enableOrder;
    bool g_enableValue;
    fable_u32 g_callbackHits;
    fable_u32 g_fontHeight;

    void Reset()
    {
        g_initialiseResult = false;
        g_sequence = 0;
        g_resetOrder = 0;
        g_initialiseCalls = 0;
        g_initialiseDisable = false;
        g_initialiseWindow = 0;
        g_allocations = 0;
        g_allocationSize = 0;
        g_appearanceCalls = 0;
        g_appearance = 0;
        g_dimensionCalls = 0;
        g_dimensionDisplay = 0;
        g_screenCalls = 0;
        g_screenWidth = 0;
        g_screenHeight = 0;
        g_finaliseCalls = 0;
        g_finaliseOrder = 0;
        g_finaliseSend = true;
        g_enableCalls = 0;
        g_enableOrder = 0;
        g_enableValue = true;
        g_callbackHits = 0;
        g_fontHeight = 0;
        ImeUiCallback_DrawRect = 0;
        ImeUiCallback_Malloc = 0;
        ImeUiCallback_Free = 0;
        ImeUiCallback_DrawFans = 0;
        ImeUiCallback_OnChar = 0;
        reinterpret_cast<CSystemManager*>(g_systemStorage)
            ->PDisplayManager.Data = kDisplay;
    }

    NUISystem::CIME& Ime(unsigned char (&storage)[sizeof(NUISystem::CIME)],
        bool enabled)
    {
        memset(storage, 0, sizeof(storage));
        storage[0] = enabled ? 1 : 0;
        return *reinterpret_cast<NUISystem::CIME*>(storage);
    }

    bool CallbacksInstalled()
    {
        if (!ImeUiCallback_DrawRect || !ImeUiCallback_Malloc ||
            !ImeUiCallback_Free || !ImeUiCallback_DrawFans ||
            !ImeUiCallback_OnChar)
            return false;
        g_callbackHits = 0;
        ImeUiCallback_DrawRect(1, 2, 3, 4, 5);
        ImeUiCallback_Free(ImeUiCallback_Malloc(8));
        ImeUiCallback_DrawFans(0, 0);
        ImeUiCallback_OnChar(L'x');
        return g_callbackHits == 0x1F;
    }

    bool CheckDisabled()
    {
        Reset();
        unsigned char storage[sizeof(NUISystem::CIME)];
        HWND__* window = reinterpret_cast<HWND__*>(0x11112222);
        Ime(storage, false).Initialise(window);
        return
            g_resetOrder == 1 &&
            g_initialiseCalls == 1 &&
            g_initialiseWindow == window &&
            g_initialiseDisable &&
            *reinterpret_cast<HWND__**>(storage + 4) == 0 &&
            ImeUiCallback_DrawRect == 0 &&
            g_allocations == 0 &&
            g_finaliseCalls == 0 &&
            g_enableCalls == 0;
    }

    bool CheckEnabledFailure()
    {
        Reset();
        unsigned char storage[sizeof(NUISystem::CIME)];
        HWND__* window = reinterpret_cast<HWND__*>(0x33334444);
        Ime(storage, true).Initialise(window);
        return
            g_resetOrder == 1 &&
            g_initialiseCalls == 1 &&
            !g_initialiseDisable &&
            g_initialiseWindow == window &&
            *reinterpret_cast<HWND__**>(storage + 4) == window &&
            storage[8] == 0 &&
            g_allocations == 0 &&
            g_appearanceCalls == 0 &&
            g_screenCalls == 0 &&
            g_finaliseCalls == 0 &&
            g_enableCalls == 0 &&
            CallbacksInstalled();
    }

    bool CheckEnabledSuccess()
    {
        Reset();
        g_initialiseResult = true;
        unsigned char storage[sizeof(NUISystem::CIME)];
        HWND__* window = reinterpret_cast<HWND__*>(0x55556666);
        Ime(storage, true).Initialise(window);
        CImeUiFont_Base* font =
            *reinterpret_cast<CImeUiFont_Base**>(storage + 0x1C);
        const bool passed =
            storage[8] == 1 &&
            g_allocations == 1 &&
            g_allocationSize == sizeof(NUISystem::CIMEFont) &&
            font != 0 &&
            g_appearanceCalls == 1 &&
            g_appearance ==
                reinterpret_cast<IMEUI_APPEARANCE*>(storage + 0x0C) &&
            g_dimensionCalls == 1 &&
            g_dimensionDisplay == kDisplay &&
            g_screenCalls == 1 &&
            g_screenWidth == 640 &&
            g_screenHeight == 480 &&
            g_finaliseCalls == 1 &&
            !g_finaliseSend &&
            g_enableCalls == 1 &&
            !g_enableValue &&
            g_finaliseOrder < g_enableOrder &&
            CallbacksInstalled();
        if (!passed)
            return false;
        font->SetHeight(24);
        const bool isFont = g_fontHeight == 24;
        free(font);
        return isFont;
    }
}

void* operator new(size_t size)
{
    ++g_allocations;
    g_allocationSize = static_cast<fable_u32>(size);
    return malloc(size);
}

void operator delete(void* pointer)
{
    free(pointer);
}

void (FABLE_FASTCALL* ImeUiCallback_DrawRect)(
    int,
    int,
    int,
    int,
    unsigned long);
void* (FABLE_FASTCALL* ImeUiCallback_Malloc)(unsigned int);
void (FABLE_FASTCALL* ImeUiCallback_Free)(void*);
void (FABLE_FASTCALL* ImeUiCallback_DrawFans)(
    const IMEUI_VERTEX*,
    unsigned int);
void (FABLE_FASTCALL* ImeUiCallback_OnChar)(wchar_t);

bool FABLE_FASTCALL ImeUi_Initialize(HWND__* window, bool disable)
{
    ++g_initialiseCalls;
    g_initialiseWindow = window;
    g_initialiseDisable = disable;
    return g_initialiseResult;
}

void FABLE_FASTCALL ImeUi_SetAppearance(const IMEUI_APPEARANCE* appearance)
{
    ++g_appearanceCalls;
    g_appearance = appearance;
}

void FABLE_FASTCALL ImeUi_SetScreenDimension(
    unsigned int width,
    unsigned int height)
{
    ++g_screenCalls;
    g_screenWidth = width;
    g_screenHeight = height;
}

void FABLE_FASTCALL ImeUi_FinalizeString(bool send)
{
    ++g_finaliseCalls;
    g_finaliseOrder = ++g_sequence;
    g_finaliseSend = send;
}

void FABLE_FASTCALL ImeUi_EnableIme(bool enable)
{
    ++g_enableCalls;
    g_enableOrder = ++g_sequence;
    g_enableValue = enable;
}

CSystemManager* FABLE_FASTCALL GFGetSystemManager()
{
    return reinterpret_cast<CSystemManager*>(g_systemStorage);
}

const C2DExtentsI CDisplayManager::GetRenderTargetDimensions()
{
    ++g_dimensionCalls;
    g_dimensionDisplay = this;
    C2DExtentsI dimensions;
    dimensions.X = 640;
    dimensions.Y = 480;
    return dimensions;
}

void NUISystem::CIME::Shutdown()
{
    g_resetOrder = ++g_sequence;
}

void FABLE_FASTCALL NUISystem::CIME::Callback_DrawRect(
    int,
    int,
    int,
    int,
    unsigned long)
{
    g_callbackHits |= 0x01;
}

void* FABLE_FASTCALL NUISystem::CIME::Callback_malloc(unsigned int bytes)
{
    g_callbackHits |= 0x02;
    return malloc(bytes);
}

void FABLE_FASTCALL NUISystem::CIME::Callback_free(void* pointer)
{
    g_callbackHits |= 0x04;
    free(pointer);
}

void FABLE_FASTCALL NUISystem::CIME::Callback_DrawFans(
    const IMEUI_VERTEX*,
    unsigned int)
{
    g_callbackHits |= 0x08;
}

void FABLE_FASTCALL NUISystem::CIME::Callback_OnChar(wchar_t)
{
    g_callbackHits |= 0x10;
}

void NUISystem::CIMEFont::SetHeight(unsigned int height)
{
    g_fontHeight = height;
}

void NUISystem::CIMEFont::SetColor(unsigned long) {}
void NUISystem::CIMEFont::SetPosition(int, int) {}
void NUISystem::CIMEFont::GetTextExtent(
    const wchar_t*,
    unsigned long*,
    unsigned long*)
{
}
void NUISystem::CIMEFont::DrawTextW(const wchar_t*) {}

int main()
{
    const bool disabled = CheckDisabled();
    const bool failure = CheckEnabledFailure();
    const bool success = CheckEnabledSuccess();
    if (!disabled || !failure || !success)
    {
        printf(
            "FABLETLC_CIME_INITIALISE_BEHAVIOR FAIL "
            "disabled=%u failure=%u success=%u\n",
            disabled,
            failure,
            success);
        return 1;
    }
    puts("FABLETLC_CIME_INITIALISE_BEHAVIOR PASS");
    return 0;
}
