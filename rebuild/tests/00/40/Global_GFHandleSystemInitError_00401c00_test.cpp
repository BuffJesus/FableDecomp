#include <stdio.h>
#include <string.h>

#include "fable_gfuninitialise.h"

namespace
{
    CSystemManager g_systemManager;
    CCharStringData g_applicationStorage;
    CCharStringData g_messageStorage;
    char g_messageBuffer[512];
    char g_displayedMessage[512];
    fable_u32 g_managerCalls = 0;
    fable_u32 g_displayCalls = 0;

    CCharStringData*& Storage(CCharString& text)
    {
        return *reinterpret_cast<CCharStringData**>(&text);
    }

    const CCharStringData* Storage(const CCharString& text)
    {
        return *reinterpret_cast<CCharStringData* const*>(&text);
    }
}

CCharString::CCharString(const char* text, long)
{
    g_applicationStorage.text = const_cast<char*>(text);
    Storage(*this) = &g_applicationStorage;
}

CCharString::~CCharString()
{
}

CCharString::operator const char*() const
{
    const CCharStringData* storage = Storage(*this);
    return storage != 0 ? storage->text : "";
}

CCharString FABLE_FASTCALL operator+(
    const CCharString& left,
    const char* right)
{
    strcpy(g_messageBuffer, static_cast<const char*>(left));
    strcat(g_messageBuffer, right);
    g_messageStorage.text = g_messageBuffer;

    CCharString result(g_messageBuffer, -1);
    Storage(result) = &g_messageStorage;
    return result;
}

extern "C" CCharString* __fastcall
FableCharStringAppendCString_0099F600(
    CCharString* result,
    const CCharString* left,
    const char* right)
{
    strcpy(g_messageBuffer, static_cast<const char*>(*left));
    strcat(g_messageBuffer, right);
    g_messageStorage.text = g_messageBuffer;
    Storage(*result) = &g_messageStorage;
    return result;
}

CSystemManager* FABLE_FASTCALL GFGetSystemManager()
{
    ++g_managerCalls;
    return &g_systemManager;
}

void CSystemManager::DisplayCriticalMessage(const char* message)
{
    ++g_displayCalls;
    strcpy(g_displayedMessage, message);
}

struct ErrorExpectation
{
    fable_u32 code;
    const char* message;
};

int main()
{
    const ErrorExpectation expectations[] = {
        {
            2,
            "Fable was unable to initialise the display. If you are running in a window, make sure the pixel depth is correct, and that your 3D card can run in a window. Use the -fullscreen option to force fullscreen, and/or -forceprimary if you have a secondary 3D accelerator you wish to ignore. Otherwise, make sure you have closed down all other applications and try again."
        },
        {4, "Fable  was unable to initialise a window. You may need to reboot your machine."},
        {5, "Fable  was unable to access the hard disc. Lord knows why."},
        {6, "Fable  was unable to initialise a joystick. Please make sure you have one connected."},
        {10, "Fable  was unable to initialise a mouse. Please make sure you have one connected."},
        {12, "Fable  was unable to initialise the input manager."},
        {
            13,
            "Fable  was unable to initialise a your 3D card. If you are running in a window, make sure the pixel depth is correct, and that your 3D card can run in a window. Otherwise, make sure you have closed down all other applications and try again."
        },
        {14, "Fable  was unable to initialise debugging information. Lord knows why."},
        {15, "Fable  was unable to initialise. Lord knows why."},
        {16, "Fable  was unable to initialise DirectX. You may need to reboot your machine."},
        {17, "Fable  was unable to initialise the network manager."},
        {99, "Unknown initialisation error. Bugger."}
    };

    for (
        fable_u32 index = 0;
        index < sizeof(expectations) / sizeof(expectations[0]);
        ++index)
    {
        g_displayedMessage[0] = '\0';
        GFHandleSystemInitError(expectations[index].code);
        if (
            strcmp(g_displayedMessage, expectations[index].message) != 0 ||
            g_managerCalls != index + 1 ||
            g_displayCalls != index + 1)
        {
            return 1;
        }
    }

    puts("FABLETLC_GF_HANDLE_SYSTEM_INIT_ERROR_BEHAVIOR PASS");
    return 0;
}
