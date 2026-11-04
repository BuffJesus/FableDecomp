#include "test_support/string_vector_fixture.h"
#include "engine/CCodeSectionManager.h"
static CCodeSectionTree* expectedTree;static unsigned long expectedKey;static _STL::_Rb_tree_node_base* result;static int calls,bad;
namespace _STL {
 template<> CCodeSectionTree::iterator CCodeSectionTree::find(const unsigned long& key){++calls;if(this!=expectedTree||key!=expectedKey)bad=1;return iterator(result);}
 template CCodeSectionTree::iterator CCodeSectionTree::find(const unsigned long&);
}
class CCodeSectionManagerTestAccess {
public:static CCodeSectionManager::CSectionDesc& Get(CCodeSectionManager& m,unsigned long key){return m.GetSectionDesc(key);}
};
int main(){
 double raw[6];memset(raw,0,sizeof(raw));CCodeSectionManager& m=*reinterpret_cast<CCodeSectionManager*>(raw);
 typedef _STL::map<unsigned long,CCodeSectionManager::CSectionDesc>::value_type Value;
 _STL::_Rb_tree_node<Value> header,node;m.SectionDesc._M_t._M_header._M_data=&header;expectedTree=&m.SectionDesc._M_t;
 node._M_value_field.second.Loaded=true;node._M_value_field.second.Size=512;result=&node;expectedKey=7;
 CCodeSectionManager::CSectionDesc& found=CCodeSectionManagerTestAccess::Get(m,7);
 if(&found!=&node._M_value_field.second||!found.Loaded||found.Size!=512||calls!=1||bad)return 1;
 result=&header;expectedKey=8;int before=live;
 CCodeSectionManager::CSectionDesc& missing=CCodeSectionManagerTestAccess::Get(m,8);
 if(missing.Loaded||missing.Size||live!=before+2||!missing.ShortName.IsEmpty()||!missing.LongName.IsEmpty())return 2;
 {CCharString marker("retained");missing.ShortName=marker;}
 missing.Loaded=true;missing.Size=99;expectedKey=0xffffffff;
 CCodeSectionManager::CSectionDesc& again=CCodeSectionManagerTestAccess::Get(m,0xffffffff);
 if(&again!=&missing||again.Loaded||again.Size||strcmp(again.ShortName.GetConstCharPtr(),"retained")||live!=before+2||calls!=3||bad)return 3;
 puts("CODE_DESC_PASS");return 0;
}