#include "fable_checksum_cache_file.h"
#include <stdio.h>
#include <string.h>
typedef _STL::map<CWideString,CChecksumCacheFile::CEntry>::_Rep_type Tree;
typedef _STL::map<CWideString,CChecksumCacheFile::CEntry>::value_type Value;
typedef _STL::_Rb_tree_node<Value> Node;
bool CWideString::operator<(const CWideString& other) const {return wcscmp(reinterpret_cast<const wchar_t*>(storage_),reinterpret_cast<const wchar_t*>(other.storage_))<0;}
int main(){
    unsigned char treeBytes[sizeof(Tree)]={0},nodeBytes[4][sizeof(Node)]={0},keyBytes[sizeof(CWideString)]={0};
    Tree* tree=reinterpret_cast<Tree*>(treeBytes);Node* nodes[4];for(unsigned i=0;i<4;++i)nodes[i]=reinterpret_cast<Node*>(nodeBytes[i]);
    tree->_M_header._M_data=nodes[0];CWideString& key=*reinterpret_cast<CWideString*>(keyBytes);*reinterpret_cast<const wchar_t**>(&key)=L"a";
    if(tree->_M_lower_bound(key)!=nodes[0])return 1;
    nodes[0]->_M_parent=nodes[2];nodes[2]->_M_left=nodes[1];nodes[2]->_M_right=nodes[3];
    const wchar_t* values[3]={L"b",L"d",L"f"};for(unsigned n=0;n<3;++n)*reinterpret_cast<const wchar_t**>(const_cast<CWideString*>(&nodes[n+1]->_M_value_field.first))=values[n];
    unsigned char before[sizeof(nodeBytes)];memcpy(before,nodeBytes,sizeof(before));
    const wchar_t* queries[7]={L"a",L"b",L"c",L"d",L"e",L"f",L"g"};unsigned expected[7]={1,1,2,2,3,3,0};
    for(unsigned q=0;q<7;++q){*reinterpret_cast<const wchar_t**>(&key)=queries[q];if(tree->_M_lower_bound(key)!=nodes[expected[q]])return 2;}
    if(memcmp(before,nodeBytes,sizeof(before)))return 3;
    puts("CACHE_LOWER_BOUND_00a609c0_PASS");return 0;
}
