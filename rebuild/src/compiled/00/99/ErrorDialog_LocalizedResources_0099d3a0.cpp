// bbblibrary/errordialog.cpp localized resource helpers (Ego_r names and
// parameters): RetrieveStringByLanguage 0099D3A0, RetrieveEnglishString
// 0099D430 (absent from the manifest), RetrieveString 0099D470,
// LocalizedDialogBoxByLanguage 0099D4D0, LocalizedDialogBox 0099D520. Each
// tries gApplicationLanguage, then US English (0x409), then the plain user32
// call. kernel32/user32 are called as dllimports; the handle types are
// declared here as <windows.h> declares them.

struct HINSTANCE__;
struct HWND__;
struct HRSRC__;
typedef int (__stdcall *DialogProcedure)(HWND__*, unsigned int, unsigned int, long);

extern "C" __declspec(dllimport) HRSRC__* __stdcall FindResourceExA(HINSTANCE__* module, const char* type,
                                                                   const char* name, unsigned short language);
extern "C" __declspec(dllimport) void* __stdcall LoadResource(HINSTANCE__* module, HRSRC__* resource);
extern "C" __declspec(dllimport) void* __stdcall LockResource(void* data);
extern "C" __declspec(dllimport) int __stdcall WideCharToMultiByte(unsigned int codePage, unsigned long flags,
                                                                  const unsigned short* wide, int wideCount,
                                                                  char* multiByte, int multiByteCount,
                                                                  const char* defaultChar, int* usedDefault);
extern "C" __declspec(dllimport) int __stdcall LoadStringA(HINSTANCE__* module, unsigned int id, char* buffer,
                                                          int bufferSize);
extern "C" __declspec(dllimport) int __stdcall DialogBoxIndirectParamA(HINSTANCE__* instance, const void* dialog,
                                                                      HWND__* parent, DialogProcedure procedure,
                                                                      long initParam);
extern "C" __declspec(dllimport) int __stdcall DialogBoxParamA(HINSTANCE__* instance, const char* templateName,
                                                              HWND__* parent, DialogProcedure procedure,
                                                              long initParam);

#define RESOURCE_TYPE_DIALOG reinterpret_cast<const char*>(5)
#define RESOURCE_TYPE_STRING reinterpret_cast<const char*>(6)
#define LANGUAGE_ENGLISH_US 0x409

extern unsigned short gApplicationLanguage;

// String tables hold 16 length-prefixed UTF-16 strings per block.
int __fastcall RetrieveStringByLanguage(HINSTANCE__* hModule, unsigned long dwStringID, char* pBuffer,
                                        unsigned long dwBufferSize, unsigned short wLanguageId)
{
    unsigned long index = dwStringID & 0xF;
    HRSRC__* resource = FindResourceExA(hModule, RESOURCE_TYPE_STRING,
        reinterpret_cast<const char*>(static_cast<unsigned short>((dwStringID >> 4) + 1)), wLanguageId);
    if (resource != 0)
    {
        void* data = LoadResource(hModule, resource);
        if (data != 0)
        {
            const unsigned short* text = static_cast<const unsigned short*>(LockResource(data));
            if (text != 0)
            {
                for (unsigned long i = 0; i < 16; ++i)
                {
                    unsigned int length = *text++;
                    if (length != 0 && i == index)
                    {
                        int written = WideCharToMultiByte(0, 0, text, length, pBuffer, dwBufferSize, 0, 0);
                        if (written == 0)
                            return 0;
                        int end = dwBufferSize - 1;
                        if (written < end)
                            end = written;
                        pBuffer[end] = 0;
                        return written;
                    }
                    text += length;
                }
            }
        }
    }
    return 0;
}

extern "C" int __fastcall RetrieveEnglishString(HINSTANCE__* hModule, unsigned long dwStringID, char* pBuffer,
                                                unsigned long dwBufferSize)
{
    int result = RetrieveStringByLanguage(hModule, dwStringID, pBuffer, dwBufferSize, LANGUAGE_ENGLISH_US);
    if (result == 0)
        result = LoadStringA(hModule, dwStringID, pBuffer, dwBufferSize);
    return result;
}

extern "C" int __fastcall RetrieveString(HINSTANCE__* hModule, unsigned long dwStringID, char* pBuffer,
                                         unsigned long dwBufferSize)
{
    int result = RetrieveStringByLanguage(hModule, dwStringID, pBuffer, dwBufferSize, gApplicationLanguage);
    if (result == 0)
    {
        if (gApplicationLanguage != LANGUAGE_ENGLISH_US)
            result = RetrieveStringByLanguage(hModule, dwStringID, pBuffer, dwBufferSize, LANGUAGE_ENGLISH_US);
        if (result == 0)
            result = LoadStringA(hModule, dwStringID, pBuffer, dwBufferSize);
    }
    return result;
}

int __fastcall LocalizedDialogBoxByLanguage(HINSTANCE__* hInstance, const char* pDialogID, HWND__* hWndParent,
                                            DialogProcedure pDialogFunc, unsigned short wLanguageId)
{
    HRSRC__* resource = FindResourceExA(hInstance, RESOURCE_TYPE_DIALOG, pDialogID, wLanguageId);
    if (resource == 0)
        return -1;
    void* data = LoadResource(hInstance, resource);
    if (data == 0)
        return -1;
    const void* dialog = LockResource(data);
    if (dialog == 0)
        return -1;
    return DialogBoxIndirectParamA(hInstance, dialog, hWndParent, pDialogFunc, 0);
}

extern "C" int __fastcall LocalizedDialogBox(HINSTANCE__* hInstance, const char* pDialogID, HWND__* hWndParent,
                                             DialogProcedure pDialogFunc)
{
    int result = LocalizedDialogBoxByLanguage(hInstance, pDialogID, hWndParent, pDialogFunc, gApplicationLanguage);
    if (result == -1)
    {
        if (gApplicationLanguage != LANGUAGE_ENGLISH_US)
            result = LocalizedDialogBoxByLanguage(hInstance, pDialogID, hWndParent, pDialogFunc, LANGUAGE_ENGLISH_US);
        if (result == -1)
            result = DialogBoxParamA(hInstance, pDialogID, hWndParent, pDialogFunc, 0);
    }
    return result;
}
