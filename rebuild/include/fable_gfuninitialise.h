#pragma once

#include "fable_system.h"

class CInitBaseClass
{
public:
    void Uninitialise();
};

class CBankFileManager
{
public:
    void Close();
};

struct CSystemManager
{
    ESystemCode Initialise(CSystemManagerInit* init);
    void Uninitialise();
    void DisplayCriticalMessage(const char* message);
};

class CThreadedFileManager
{
public:
    void Uninitialise();
};

class CEnginePrimitiveManager
{
public:
    void Uninitialise();
};

extern CInitBaseClass g_FableInitBase_013B83D0;
extern CBankFileManager g_FableBankFileManager_013CA79C;
extern CThreadedFileManager g_FableThreadedFileManager_013BC870;

void FABLE_FASTCALL FableFileInstallerSingletonRelease();
CSystemManager* FABLE_FASTCALL GFGetSystemManager();
void FABLE_FASTCALL UninitializeStreamingFontTextureCache();
CEnginePrimitiveManager* FABLE_FASTCALL
FableGetEnginePrimitiveManager_00404A80();

void FABLE_CDECL GFUninitialise();
void FABLE_STDCALL GFHandleSystemInitError(unsigned long errorCode);
