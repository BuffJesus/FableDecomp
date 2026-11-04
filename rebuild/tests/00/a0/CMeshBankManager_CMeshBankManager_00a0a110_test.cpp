#include <stdio.h>
#include <string.h>
#include <new>
#include "fable_system_manager.h"
#include "test_support/base_class_stubs.h"
#include "test_support/bank_manager_member_stubs.h"
static int baseCalls;
CBaseClassNonCopyable::CBaseClassNonCopyable(){++baseCalls;}CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
void CMeshBankManager::OnPreDeviceReset(){}bool CMeshBankManager::OnPostDeviceReset(){return true;}
struct CBankManagerTestAccess {static _STL::_List_node_base* node(CGraphicBankManager& p){return p.GraphicDataBanks._M_node;}static _STL::_List_node_base* node(CMeshBankManager& p){return p.MeshDataBanks._M_node;}};
#define CHECK(x) do{if(!(x)){printf("FAIL %d\n",__LINE__);return 1;}}while(0)
int main(){union {double align;unsigned char bytes[32];}u;memset(u.bytes,0xa5,sizeof(u.bytes));CMeshBankManager* p=new(u.bytes+4)CMeshBankManager;CHECK(sizeof(*p)==12&&baseCalls==1);_STL::_List_node_base* n=CBankManagerTestAccess::node(*p);CHECK(n&&n->_M_next==n&&n->_M_prev==n);for(int i=0;i<4;++i)CHECK(u.bytes[i]==0xa5&&u.bytes[12+4+i]==0xa5);CHECK((char*)static_cast<CDeviceResetCallback*>(p)==(char*)p+4);CHECK(static_cast<CDeviceResetCallback*>(p)->OnPostDeviceReset()); p->~CMeshBankManager();puts("BANK_MANAGER_CTOR_PASS");return 0;}