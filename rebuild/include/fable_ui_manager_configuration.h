#pragma once
#include "fable_ui_observer.h"
#include "fable_ui_observer_events.h"

// Retail integer map storage; same tree links as the recovered event set.
struct FableUiIntegerMapStorage
{
    FableUiEventTreeNode* Head;
    unsigned Count;
    unsigned char Unrecovered08[4];
};
struct FableUiIntegerMapNode { FableUiEventTreeNode LinksAndKey; int Value; };
struct FableUiThingActionNode { FableUiThingActionNode* Next; FableUiThingActionNode* Previous; int Parameter; };
struct FableUiActionParameterNode { FableUiActionParameterNode* Next; FableUiActionParameterNode* Previous; void* Parameter; };
FABLE_STATIC_ASSERT(sizeof(FableUiIntegerMapNode)==24);
FABLE_STATIC_ASSERT(offsetof(FableUiIntegerMapNode,Value)==20);
struct FableUiManagerConfigurationView
{
    FableUiManagerObserverView Observable;
    void* GraphicsBank;
    void* GraphicsBankReference;
    void* MeshBank;
    void* MeshBankReference;
    void* InputManager;
    FableUiIntegerMapStorage Keys;
    int InputType;
    void* HeroExperience;
    void* AbilitiesInventory;
    void* WeaponsInventory;
    void* MapInventory;
    void* QuestInventory;
    void* ExperienceInventory;
    void* Inventory;
    void* ClothingInventory;
    void* TradeInventory;
    void* Player;
    FableUiThingActionNode* ThingActionParams;
    FableUiActionParameterNode* UIActionParams;
    void* DefinitionManager;
    unsigned NumberAugmentations;
    int Change;
    FableUiIntegerMapStorage Layers;
    int MetaLayer;
};
FABLE_STATIC_ASSERT(offsetof(FableUiManagerConfigurationView, Keys)==0x24);
FABLE_STATIC_ASSERT(offsetof(FableUiManagerConfigurationView, InputType)==0x30);
FABLE_STATIC_ASSERT(offsetof(FableUiManagerConfigurationView, Layers)==0x70);
FABLE_STATIC_ASSERT(offsetof(FableUiManagerConfigurationView, MetaLayer)==0x7C);
// Retail 0042D201 and 0042D246 insert missing entries and return their values.
void* FableUiAllocateIntegerMapNode(unsigned);
int* __fastcall FableUiLookupInputKey(FableUiIntegerMapStorage*, void*, const int*);
int* __fastcall FableUiLookupLayer(FableUiIntegerMapStorage*, void*, const int*);
void __fastcall FableUiSetInput(FableUiManagerConfigurationView*, void*, int);
void __fastcall FableUiSetMetaLayer(FableUiManagerConfigurationView*, void*, int);

inline int* FableUiFindOrInsertIntegerMap(FableUiIntegerMapStorage* map,const int* key)
{
    FableUiEventTreeNode* parent=map->Head;
    FableUiEventTreeNode* node=map->Head->Parent;
    while(node)
    {
        parent=node;
        if(*key==node->Event) return &reinterpret_cast<FableUiIntegerMapNode*>(node)->Value;
        node=*key<node->Event ? node->Left : node->Right;
    }
    FableUiIntegerMapNode* inserted=static_cast<FableUiIntegerMapNode*>(FableUiAllocateIntegerMapNode(24));
    node=&inserted->LinksAndKey;
    node->Event=*key; inserted->Value=0;
    if(parent==map->Head || *key<parent->Event)
    {
        parent->Left=node;
        if(parent==map->Head) { map->Head->Parent=node; map->Head->Right=node; }
        else if(parent==map->Head->Left) map->Head->Left=node;
    }
    else
    {
        parent->Right=node;
        if(parent==map->Head->Right) map->Head->Right=node;
    }
    node->Parent=parent; node->Left=0; node->Right=0;
    FableUiBalanceEventTree(node,&map->Head->Parent);
    ++map->Count;
    return &inserted->Value;
}
