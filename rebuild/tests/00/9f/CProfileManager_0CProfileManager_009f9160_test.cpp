#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_system_manager.h"
#include "../../../src/compiled/00/43/STL_DefStringMapErase_0043020a.cpp"
CCharString::~CCharString() {}
CProfileManager* g_FableProfileManager_013CB4F8=0;
static double systemRaw[(sizeof(CSystemManager)+7)/8];
static CSystemManager* systemManager=reinterpret_cast<CSystemManager*>(systemRaw);
static int systemCalls,insertCalls;
CSystemManager* FABLE_FASTCALL GFGetSystemManager(){++systemCalls;return systemManager;}
typedef CProfileManager::CProfileTreeEntry Entry;
static double entryRaw[(sizeof(_STL::_List_node<Entry>)+7)/8];
static _STL::_List_node<Entry>* node=reinterpret_cast<_STL::_List_node<Entry>*>(entryRaw);
namespace _STL {
 template<> list<Entry>::iterator list<Entry>::insert(iterator pos){
  ++insertCalls;if(pos._M_node!=_M_node || _M_node->_M_next!=_M_node || _M_node->_M_prev!=_M_node)abort();
  memset(entryRaw,0xcc,sizeof(entryRaw));node->_M_next=_M_node;node->_M_prev=_M_node;_M_node->_M_next=node;_M_node->_M_prev=node;
  return iterator(node);
 }
}
struct CProfileManagerTestAccess {
 static int Check(CProfileManager* m,bool display){
  if(m->UpPressed || m->DownPressed || m->TogglePressed || m->CursorPosition._M_node || m->NeedsSaving || m->MinUpdated || m->MinApplicationTotalTime!=0.0 || m->NoTurns || m->DrawMode || m->EvalMode!=1)return 1;
  if(m->CurrentTreeEntry!=&node->_M_data || node->_M_data.Parent)return 2;
  if(display && m->RenderManager!=reinterpret_cast<CRenderManager2D*>(0x123400))return 3;
  _STL::_List_node_base* list=m->ProfileList._M_node;
  if(!list || list->_M_next!=list || list->_M_prev!=list)return 4;
  _STL::_Rb_tree_node_base* maps[]={m->IDToProfileName._M_t._M_header._M_data,m->ProfileNameToID._M_t._M_header._M_data};
  for(int i=0;i<2;++i)if(!maps[i] || maps[i]->_M_color || maps[i]->_M_parent || maps[i]->_M_left!=maps[i] || maps[i]->_M_right!=maps[i])return 5;
  if(m->IDToProfileName._M_t._M_node_count || m->ProfileNameToID._M_t._M_node_count)return 6;
  const int ranges[][2]={{0,3},{4,4},{0x20,1},{0x28,8},{0x38,4},{0x40,8},{0x4c,12},{0x5c,8},{0x68,9}};
  unsigned char expected[sizeof(CProfileManager)];memset(expected,0xcd,sizeof(expected));
  for(int j=0;j<sizeof(ranges)/sizeof(ranges[0]);++j)memcpy(expected+ranges[j][0],reinterpret_cast<unsigned char*>(m)+ranges[j][0],ranges[j][1]);
  if(display)memcpy(expected+0x3c,reinterpret_cast<unsigned char*>(m)+0x3c,4);
  if(memcmp(expected,m,sizeof(expected)))return 7;
  free(m->ProfileList._M_node);free(m->ProfileTree._M_node);free(maps[0]);free(maps[1]);return 0;
 }
};
int main(){
 for(int display=0;display<2;++display)for(int existing=0;existing<2;++existing){
  double storage[(sizeof(CProfileManager)+16+7)/8];memset(storage,0xcd,sizeof(storage));
  double displayRaw[(sizeof(CDisplayManager)+7)/8];memset(displayRaw,0,sizeof(displayRaw));CDisplayManager* dm=reinterpret_cast<CDisplayManager*>(displayRaw);new (&dm->PRenderManager) boost::scoped_ptr<CRenderManager>(reinterpret_cast<CRenderManager*>(reinterpret_cast<CRenderManager2D*>(0x123400)));
  memset(systemRaw,0,sizeof(systemRaw));systemManager->PDisplayManager.Data=display?dm:0;
  CProfileManager* prior=reinterpret_cast<CProfileManager*>(0x987600);g_FableProfileManager_013CB4F8=existing?prior:0;systemCalls=insertCalls=0;
  CProfileManager* m=new(reinterpret_cast<char*>(storage)+8) CProfileManager;
  if(systemCalls!=(display?2:1) || insertCalls!=1 || g_FableProfileManager_013CB4F8!=(existing?prior:m))return 10;
  int result=CProfileManagerTestAccess::Check(m,display!=0);if(result){printf("check %d\n",result);return result;}
  for(int i=0;i<8;++i)if(reinterpret_cast<unsigned char*>(storage)[i]!=0xcd || reinterpret_cast<unsigned char*>(m)[sizeof(*m)+i]!=0xcd)return 11;
 }
 puts("PROFILE_CTOR_PASS");return 0;
}
