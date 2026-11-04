#include <new>
#include <stdio.h>
#include <string.h>
#include "engine/CCodeSectionManager.h"
static void FreeHeaders(CCodeSectionManager& m){
 free(m.ShortNameToHandle._M_t._M_header._M_data);
 free(m.LongNameToHandle._M_t._M_header._M_data);
 free(m.SectionDesc._M_t._M_header._M_data);
}
CCodeSectionManager::~CCodeSectionManager(){FreeHeaders(*this);}
template<class Map> bool valid(Map& m){
 _STL::_Rb_tree_node_base* h=m._M_t._M_header._M_data;
 return h && !h->_M_color && !h->_M_parent && h->_M_left==h && h->_M_right==h && m._M_t._M_node_count==0;
}
int main(){
 double raw[6];memset(raw,0xa5,sizeof(raw));CCodeSectionManager* m=new(raw)CCodeSectionManager;
 if(m->SectionLoadingEnabled||!valid(m->ShortNameToHandle)||!valid(m->LongNameToHandle)||!valid(m->SectionDesc))return 1;
 void* a=m->ShortNameToHandle._M_t._M_header._M_data;void* b=m->LongNameToHandle._M_t._M_header._M_data;void* c=m->SectionDesc._M_t._M_header._M_data;
 if(a==b||b==c||a==c)return 2;
 const unsigned char* bytes=(const unsigned char*)raw;for(unsigned i=sizeof(*m);i<sizeof(raw);++i)if(bytes[i]!=0xa5)return 3;
 m->~CCodeSectionManager();puts("CODE_CTOR_PASS");return 0;
}