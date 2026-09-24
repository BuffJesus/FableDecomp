#include "fable_ui_manager_construction.h"

template<class Node> static void ConstructParameterList(Node** list)
{
    *list=0;
    Node* head=static_cast<Node*>(FableUiAllocateManagerStorage(12));
    head->Next=head; head->Previous=head; *list=head;
}
FableUiManagerView* __fastcall FableUiConstructManager(FableUiManagerView* manager,void*)
{
    FableUiManagerConfigurationView& config=manager->Configuration;
    FableUiConstructObservable(&config.Observable,0);
    config.Observable.Vtable=&FableUiManagerVtable;
    config.GraphicsBank=0; config.GraphicsBankReference=0; config.MeshBank=0; config.MeshBankReference=0;
    FableUiConstructInputMap(&config.Keys,0);
    ConstructParameterList(&config.ThingActionParams);
    ConstructParameterList(&config.UIActionParams);
    config.DefinitionManager=0;
    FableUiConstructLayerMap(&config.Layers,0);
    manager->BastardChild.Data=0; manager->BastardChild.Info=0;
    FableUiConstructComponentMap(&manager->BastardChildren,0);
    manager->LastRenderTime=0.0; manager->HasErrorMessage=false; manager->ErrorMessage=0; manager->Restoring=false;
    manager->MouseCursor=0; manager->Dropped=0; manager->DescriptionScrollingOffset=-1.0f;
    manager->DescriptionScrollingToMax=false; manager->MouseDisabled=false;
    manager->AssignmentItemsExpressionsSwapping=false; manager->AssignmentSpellsSwapping=false;
    config.HeroExperience=0; config.AbilitiesInventory=0; config.WeaponsInventory=0; config.MapInventory=0;
    config.QuestInventory=0; config.ExperienceInventory=0; config.Inventory=0; config.ClothingInventory=0;
    config.TradeInventory=0; config.Player=0;
    FableUiSetRelativeCoordinates(true);
    FableUiSetInput(&config,0,0);
    config.WeaponsInventory=0;

    FableUiGraphicBankInit init;
    FableUiConstructGraphicBankInit(&init,0);
    FableUiChooseNonAlphaFormat(FableUiDisplayManager,0,16,&init.NonAlphaFormat);
    FableUiChooseAlphaFormat(FableUiDisplayManager,0,16,&init.AlphaFormat);
    FableUiChooseBooleanAlphaFormat(FableUiDisplayManager,0,16,&init.BooleanAlphaFormat);
    FableUiChooseUncompressedAlphaFormat(FableUiDisplayManager,0,32,&init.UncompressedAlphaFormat);
    FableUiChooseUncompressedNonAlphaFormat(FableUiDisplayManager,0,32,&init.UncompressedNonAlphaFormat,false);
    FableUiChooseSignedFormat(FableUiDisplayManager,0,16,&init.SignedFormat);
    init.GenerateMipmaps=false; init.MaxGraphicWidth=init.MaxGraphicHeight=~0u; init.AllowDither=false;
    const char* bankName=FableUiGraphicMode && FableUiGraphicMode->Frontend ? "GBANK_FRONT_END" : "GBANK_MAIN";
    void* banks=FableUiGetSystemManager()->GraphicsBankManager;
    FableUiStringValue name;
    FableUiConstructBankName(&name,0,bankName,-1);
    FableUiBankReference bank;
    FableUiCreateGraphicsBank(banks,0,&bank,&name,&init,true,true);
    FableUiSetGraphicsBank(manager,0,bank);
    FableUiDestroyBankName(&name,0);
    FableUiSetMetaLayer(&config,0,0);
    return manager;
}

// Preserve the singleton's historical external binding while using the actual
// CManager constructor identified by retail RTTI.
extern "C" void* __fastcall FableFrontEndManagerConstruct(void* storage)
{ return FableUiConstructManager(static_cast<FableUiManagerView*>(storage),0); }
