#include "engine/CGameScriptInterface.h"

struct CGameScriptInterface_Methods : CGameScriptInterface { void GSI_PreMsg(); void GSI_PostMsg(); };
struct CMgr { void* Find(int* pKey); void Handle(void* pObj); };
bool __fastcall CGameScriptInterface_MsgOnChestOpeningCancelled(CGameScriptInterface* self)
{
 CGameScriptInterface_Methods* methods=(CGameScriptInterface_Methods*)self;
 methods->GSI_PreMsg();methods->GSI_PostMsg();
 CMgr* mgr=*(CMgr**)((char*)self->World+0x60);int key=0x48;void* obj=mgr->Find(&key);
 if(obj){mgr->Handle(obj);return true;}return false;
}
