#pragma once

#include "fable_string.h"
#include "rebuild_abi.h"

class CGame;

class CGameComponent
{
public:
    virtual ~CGameComponent();
    virtual void Init();
    virtual bool Run(CGameComponent** nextComponent);
};

class CMainGameComponentInit
{
public:
    CWideString firstWideString;
    CWideString secondWideString;
    CCharString narrowString;
    CWideString fourthWideString;
};

struct CFrontendGameComponentInit
{
    fable_u32 value;
};

typedef CFrontendGameComponentInit CNewFrontendGameComponentInit;

class CMainGameComponent : public CGameComponent
{
public:
    static void FABLE_FASTCALL GenerateMetFilesFromLugFiles();

    CMainGameComponent(
        CGame& game,
        const CMainGameComponentInit& init);

private:
    fable_u8 unknown04_[0x161E4];
};

class CFrontendGameComponent : public CGameComponent
{
public:
    CFrontendGameComponent(
        CGame& game,
        const CFrontendGameComponentInit& init);

private:
    fable_u8 unknown04_[0x1E5C];
};

class CNewFrontendGameComponent : public CGameComponent
{
public:
    CNewFrontendGameComponent(
        CGame& game,
        const CNewFrontendGameComponentInit& init);

    static void CompileDefs();

private:
    fable_u8 unknown04_[0x144];
};

class CGame
{
public:
    void Play();

private:
    fable_u8 unknown00_[8];
    CGameComponent* component_;
    fable_u8 unknown0C_[0x200];
    bool quit_;
};

extern fable_u8 g_FableCompileFrontendDefinitions_013B8648;
extern fable_u8 g_FableStartMainGame_013B8605;
extern fable_u8 g_FableUseLegacyFrontend_013B8642;
extern CGameComponent* g_FableRetiredGameComponent_013B7D58;
extern CWideString g_FableMainGameStartupPath_013B7D5C;

FABLE_STATIC_ASSERT(sizeof(CMainGameComponentInit) == 0x10);
FABLE_STATIC_ASSERT(sizeof(CFrontendGameComponentInit) == 0x04);
FABLE_STATIC_ASSERT(sizeof(CMainGameComponent) == 0x161E8);
FABLE_STATIC_ASSERT(sizeof(CFrontendGameComponent) == 0x1E60);
FABLE_STATIC_ASSERT(sizeof(CNewFrontendGameComponent) == 0x148);
FABLE_STATIC_ASSERT(sizeof(CGame) == 0x210);
