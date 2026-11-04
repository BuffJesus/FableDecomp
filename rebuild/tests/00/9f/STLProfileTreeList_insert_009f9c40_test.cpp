#include "fable_system_manager.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef CProfileManager::CProfileTreeEntry Entry;
static char calls[16];static _STL::list<Entry>* expectedList;static _STL::_List_node_base *position,*resultNode;static const CWideString* tempName;
CWideString::CWideString(){strcat(calls,"c");tempName=this;}
CWideString::~CWideString(){if(this!=tempName)abort();strcat(calls,"d");}
namespace _STL {
 template<> list<Entry>::iterator list<Entry>::insert(iterator at,const Entry& value){
  if(this!=expectedList || at._M_node!=position || value.Profile.Manager || &value.Profile.Name!=tempName || value.Children._M_start || value.Children._M_finish || value.Children._M_end_of_storage._M_data)abort();
  strcat(calls,"i");return iterator(resultNode);
 }
}
int main(){
 double raw[(sizeof(_STL::list<Entry>)+7)/8];expectedList=reinterpret_cast<_STL::list<Entry>*>(raw);
 _STL::_List_node_base sentinel,other;expectedList->_M_node=reinterpret_cast<_STL::list<Entry>::_Node*>(&sentinel);
 for(int i=0;i<2;++i){position=i?&other:&sentinel;resultNode=i?&sentinel:&other;calls[0]=0;
  _STL::list<Entry>::iterator result=expectedList->insert(_STL::list<Entry>::iterator(position));
  if(result._M_node!=resultNode || strcmp(calls,"cid") || expectedList->_M_node!=reinterpret_cast<_STL::list<Entry>::_Node*>(&sentinel))return 1;
 }
 puts("PROFILE_DEFAULT_INSERT_PASS");return 0;
}