#include "test_support/string_vector_fixture.h"
#include "engine/CCodeSectionManager.h"
typedef _STL::map<unsigned long,CCodeSectionManager::CSectionDesc>::value_type Value;
typedef _STL::_Rb_tree_node<Value> Node;
int main(){
 CCodeSectionTree tree;unsigned long absent=7;_STL::_Rb_tree_node_base* header=tree._M_header._M_data;
 if(tree.find(absent)._M_node!=header)return 1;
 const unsigned long keys[]={0,1,0x7fffffff,0x80000000,0xfffffffe,0xffffffff};Node* nodes[6];
 {CCodeSectionManager::CSectionDesc desc;desc.Loaded=false;desc.Size=0;
  for(int i=0;i<6;++i){nodes[i]=(Node*)malloc(sizeof(Node));memset(nodes[i],0,sizeof(Node));new(&nodes[i]->_M_value_field)Value(keys[i],desc);}}
 header->_M_parent=nodes[3];nodes[3]->_M_left=nodes[1];nodes[1]->_M_left=nodes[0];nodes[1]->_M_right=nodes[2];nodes[3]->_M_right=nodes[4];nodes[4]->_M_right=nodes[5];tree._M_node_count=6;
 for(int i=0;i<6;++i)if(tree.find(keys[i])._M_node!=nodes[i])return 2;
 const unsigned long misses[]={2,0x7ffffffe,0x80000001,0xfffffffd};
 for(i=0;i<4;++i)if(tree.find(misses[i])._M_node!=header)return 3;
 if(tree._M_node_count!=6||header->_M_parent!=nodes[3]||nodes[3]->_M_left!=nodes[1]||nodes[4]->_M_right!=nodes[5])return 4;
 for(i=0;i<6;++i){nodes[i]->_M_value_field.~Value();free(nodes[i]);}free(header);
 if(live||dataLive)return 5;puts("CODE_FIND_PASS");return 0;
}