#ifndef FABLE_DINPUT8_H
#define FABLE_DINPUT8_H

// DirectInput 8 declarations for engine sources (as fable_d3d9.h does for Direct3D): the real <dinput.h> when the
// Windows SDK is in the translation unit, otherwise the SDK's shape - the same interface, methods and slot order, so
// COM calls compile to the retail `call [reg+slot]` sequences. IDirectInputDevice8W is declared up to
// SetCooperativeLevel (slot 13); later slots are not modelled yet (append in SDK order).

#if defined(_WINDOWS_) || defined(__DINPUT_INCLUDED__)

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif
#include <dinput.h>

#else

typedef long HRESULT;
typedef unsigned long DWORD;
struct _GUID
{
    unsigned long Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char Data4[8];
};
typedef struct _GUID GUID;
typedef const GUID& REFIID;

#define DI_OK       ((HRESULT)0L)
#define DI_NOEFFECT ((HRESULT)1L)
#ifndef SUCCEEDED
#define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)
#endif

// One buffered input record (dinput.h, DIRECTINPUT_VERSION 0x0800: 0x14 bytes).
typedef struct DIDEVICEOBJECTDATA
{
    DWORD dwOfs;
    DWORD dwData;
    DWORD dwTimeStamp;
    DWORD dwSequence;
    unsigned int uAppData;
} DIDEVICEOBJECTDATA;

// Property headers (dinput.h): GetProperty fills a DIPROPSTRING for DIPROP_KEYNAME (CKeyboardDX::GetKeyName 0x00AB6180).
typedef struct DIPROPHEADER
{
    DWORD dwSize;
    DWORD dwHeaderSize;
    DWORD dwObj;
    DWORD dwHow;
} DIPROPHEADER;

typedef struct DIPROPSTRING
{
    DIPROPHEADER diph;
    unsigned short wsz[260];   // WCHAR (unsigned short without /Zc:wchar_t)
} DIPROPSTRING;

#define DIPH_BYOFFSET 1
#define MAKEDIPROP(prop) (*(const GUID*)(prop))
#define DIPROP_KEYNAME MAKEDIPROP(20)
#define DIPROP_BUFFERSIZE MAKEDIPROP(1)
#define DIPROP_RANGE MAKEDIPROP(4)
#define DIPROP_DEADZONE MAKEDIPROP(5)
#define DIPH_DEVICE 0
#define DI_PROPNOEFFECT ((HRESULT)1L)
#define DIJOFS_X 0
#define DIJOFS_Y 4
#define DISCL_EXCLUSIVE 0x00000001
#define DISCL_FOREGROUND 0x00000004

typedef struct DIPROPDWORD
{
    DIPROPHEADER diph;
    DWORD dwData;
} DIPROPDWORD;

typedef struct DIPROPRANGE
{
    DIPROPHEADER diph;
    long lMin;
    long lMax;
} DIPROPRANGE;

// Only the instance GUID is read (CJoystickDX::EnumAndInit 0x00AB6B10).
struct DIDEVICEINSTANCEW
{
    DWORD dwSize;
    GUID guidInstance;
};

typedef struct DIDATAFORMAT
{
    DWORD dwSize;
    DWORD dwObjSize;
    DWORD dwFlags;
    DWORD dwDataSize;
    DWORD dwNumObjs;
    void* rgodf;
} DIDATAFORMAT;
// dinput8.lib's joystick data format (retail copy 0x012AE20C).
extern "C" const DIDATAFORMAT c_dfDIJoystick;

// IDirectInput8::EnumDevices (CJoystickDX::SetExclusiveMode 0x00AB6DE0).
#define DI8DEVCLASS_GAMECTRL 4
#define DIEDFL_ATTACHEDONLY 0x00000001
#define DIENUM_STOP 0
#define DIENUM_CONTINUE 1
struct DIDEVICEINSTANCEW;
typedef int (__stdcall* LPDIENUMDEVICESCALLBACKW)(const DIDEVICEINSTANCEW* instance, void* ref);

struct IDirectInputDevice8W
{
    virtual HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObj) = 0;           // [0]
    virtual unsigned long __stdcall AddRef() = 0;                                      // [1]
    virtual unsigned long __stdcall Release() = 0;                                     // [2]
    virtual HRESULT __stdcall GetCapabilities(void* lpDIDevCaps) = 0;                  // [3]
    virtual HRESULT __stdcall EnumObjects(void* lpCallback, void* pvRef, DWORD dwFlags) = 0; // [4]
    virtual HRESULT __stdcall GetProperty(REFIID rguidProp, void* pdiph) = 0;          // [5]
    virtual HRESULT __stdcall SetProperty(REFIID rguidProp, const void* pdiph) = 0;    // [6]
    virtual HRESULT __stdcall Acquire() = 0;                                           // [7]
    virtual HRESULT __stdcall Unacquire() = 0;                                         // [8]
    virtual HRESULT __stdcall GetDeviceState(DWORD cbData, void* lpvData) = 0;         // [9]
    virtual HRESULT __stdcall GetDeviceData(DWORD cbObjectData, DIDEVICEOBJECTDATA* rgdod,
                                            DWORD* pdwInOut, DWORD dwFlags) = 0;      // [10]
    virtual HRESULT __stdcall SetDataFormat(const DIDATAFORMAT* lpdf) = 0;            // [11]
    virtual HRESULT __stdcall SetEventNotification(void* hEvent) = 0;                // [12]
    virtual HRESULT __stdcall SetCooperativeLevel(void* hwnd, DWORD dwFlags) = 0;     // [13]
};

// IDirectInput8W is declared up to EnumDevices (slot 4); later slots are not modelled yet.
struct IDirectInput8W
{
    virtual HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObj) = 0;           // [0]
    virtual unsigned long __stdcall AddRef() = 0;                                      // [1]
    virtual unsigned long __stdcall Release() = 0;                                     // [2]
    virtual HRESULT __stdcall CreateDevice(REFIID rguid, IDirectInputDevice8W** device, void* outer) = 0; // [3]
    virtual HRESULT __stdcall EnumDevices(DWORD devType, LPDIENUMDEVICESCALLBACKW callback, void* ref,
                                          DWORD flags) = 0;                           // [4]
};

#endif

#endif
