#pragma once
#include "fable_ui_manager_configuration.h"
#include "fable_ui_display_services.h"
#include "fable_ui_strings.h"

struct FableUiBankReference { void* Data; void* Info; };
struct FableUiComponentMapStorage { FableUiEventTreeNode* Head; unsigned Count; unsigned char Unrecovered08[4]; };
struct FableUiManagerView
{
    FableUiManagerConfigurationView Configuration;
    FableUiBankReference BastardChild;
    FableUiComponentMapStorage BastardChildren;
    FableUiStateVector2 RightStickForce;
    bool HasErrorMessage;
    unsigned char Unrecovered9D[3];
    void* ErrorMessage;
    bool Restoring;
    unsigned char UnrecoveredA5[3];
    double LastRenderTime;
    FableUiStateVector2 MouseMovement;
    void* MouseCursor;
    void* Dropped;
    unsigned short KeyPressed;
    unsigned char UnrecoveredC2[2];
    float DescriptionScrollingOffset;
    bool DescriptionScrollingToMax;
    bool MouseDisabled;
    bool AssignmentItemsExpressionsSwapping;
    bool AssignmentSpellsSwapping;
    unsigned char UnrecoveredCC[4];
};
FABLE_STATIC_ASSERT(sizeof(FableUiManagerView)==0xD0);
FABLE_STATIC_ASSERT(offsetof(FableUiManagerView,BastardChildren)==0x88);
FABLE_STATIC_ASSERT(offsetof(FableUiManagerView,LastRenderTime)==0xA8);
FABLE_STATIC_ASSERT(offsetof(FableUiManagerView,DescriptionScrollingOffset)==0xC4);
struct FableUiGraphicBankInit
{
    int NonAlphaFormat,AlphaFormat,InterpolatedAlphaFormat,BooleanAlphaFormat;
    int UncompressedNonAlphaFormat,UncompressedAlphaFormat,SignedFormat;
    bool GenerateMipmaps;
    unsigned char Unrecovered1D[3];
    unsigned MaxGraphicWidth,MaxGraphicHeight;
    bool AllowDither,AllowCompressedTextures,AllowDXT1ForBooleanAlpha;
    unsigned char Unrecovered2B;
};
FABLE_STATIC_ASSERT(sizeof(FableUiGraphicBankInit)==44);
struct FableUiGraphicModeContext { unsigned char Unrecovered00[9]; bool Frontend; };
extern FableUiManagerObserverVtable FableUiManagerVtable;
extern void* FableUiDisplayManager; // 013B8390
extern FableUiGraphicModeContext* FableUiGraphicMode; // 013B871C
void* FableUiAllocateManagerStorage(unsigned);
FableUiIntegerMapStorage* __fastcall FableUiConstructInputMap(FableUiIntegerMapStorage*,void*);
FableUiIntegerMapStorage* __fastcall FableUiConstructLayerMap(FableUiIntegerMapStorage*,void*);
FableUiComponentMapStorage* __fastcall FableUiConstructComponentMap(FableUiComponentMapStorage*,void*);
FableUiGraphicBankInit* __fastcall FableUiConstructGraphicBankInit(FableUiGraphicBankInit*,void*);
FableUiManagerView* __fastcall FableUiConstructManager(FableUiManagerView*,void*);
// Display format services 009BE830/870/8B0/6C0/610/590. Names describe the
// destination fields at this call site; bool return values are ignored here.
bool __fastcall FableUiChooseNonAlphaFormat(void*,void*,int,int*);
bool __fastcall FableUiChooseAlphaFormat(void*,void*,int,int*);
bool __fastcall FableUiChooseBooleanAlphaFormat(void*,void*,int,int*);
bool __fastcall FableUiChooseUncompressedAlphaFormat(void*,void*,int,int*);
bool __fastcall FableUiChooseUncompressedNonAlphaFormat(void*,void*,int,int*,bool);
bool __fastcall FableUiChooseSignedFormat(void*,void*,int,int*);
// String and bank factory/ownership boundaries 0099EBF0/0099EAE0,
// 009F83D0 and 0042A9B7. The setter consumes a counted reference by value.
FableUiBankReference* __fastcall FableUiCreateGraphicsBank(void*,void*,FableUiBankReference*,const FableUiStringValue*,const FableUiGraphicBankInit*,bool,bool);
void __fastcall FableUiSetGraphicsBank(FableUiManagerView*,void*,FableUiBankReference);

template<class Map> inline Map* FableUiInitializeManagerMap(Map* map,unsigned nodeSize)
{
    map->Head=0;
    map->Head=static_cast<FableUiEventTreeNode*>(FableUiAllocateManagerStorage(nodeSize));
    map->Count=0; map->Head->Colour=0; map->Head->Parent=0;
    map->Head->Left=map->Head; map->Head->Right=map->Head;
    return map;
}
