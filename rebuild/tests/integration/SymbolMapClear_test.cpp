#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_symbol_map.h"
#include "test_support/base_class_stubs.h"
typedef _STL::_Rb_tree_node<CCharString> Node;
typedef _STL::pair<CCharString,CSymbolMap::CSymbolInfo> Entry;
struct Ref{int count;};static Ref refs[8];static CSymbolMap* owner;static int baseCalls,destroyCalls,deleteCalls;static const void* order[40];static const void* expected[40];static int expectedCount;
void __cdecl operator delete(void* p){if(owner){if(p!=owner)abort();++deleteCalls;}free(p);}
CBaseClassNonCopyable::CBaseClassNonCopyable(){++baseCalls;}
CBaseClassNonCopyable::~CBaseClassNonCopyable(){if(this!=owner||destroyCalls!=expectedCount)abort();}
CCharString::~CCharString(){if(owner){if(destroyCalls>=40)abort();order[destroyCalls++]=this;if(storage_)--((Ref*)storage_)->count;storage_=0;}}
CCharString& CCharString::operator=(const CCharString&){abort();return *this;}
static Node* nodes[8];
static Node* build(int lo,int hi,int shape,Node* parent){if(lo==hi)return 0;int i=shape==0?(lo+hi)/2:shape==1?lo:hi-1;Node* p=nodes[i];p->_M_parent=parent;p->_M_left=build(lo,i,shape,p);p->_M_right=build(i+1,hi,shape,p);return p;}
static void traversal(Node* node){if(!node)return;traversal((Node*)node->_M_right);expected[expectedCount++]=&node->_M_value_field;traversal((Node*)node->_M_left);}
int main(){int cases=0;
for(int count=0;count<=8;++count)for(int entries=0;entries<=8;++entries)for(int shape=0;shape<3;++shape)for(int mode=0;mode<2;++mode)for(int alias=0;alias<3;++alias){
 owner=0;baseCalls=0;CSymbolMap* map=new CSymbolMap;if(baseCalls!=1)return 1;owner=map;destroyCalls=deleteCalls=expectedCount=0;
 for(int r=0;r<8;++r)refs[r].count=1;
 for(int i=0;i<count;++i){nodes[i]=(Node*)malloc(sizeof(Node));memset(nodes[i],0,sizeof(Node));Ref* ref=alias==0?0:&refs[alias==1?0:i%8];*(Ref**)&nodes[i]->_M_value_field=ref;if(ref)++ref->count;}
 Node* header=(Node*)map->NamesUsed._M_t._M_header._M_data;header->_M_parent=build(0,count,shape,header);map->NamesUsed._M_t._M_node_count=count;traversal((Node*)header->_M_parent);
 Entry* data=entries?(Entry*)malloc((entries+3)*sizeof(Entry)):0;map->SLONGMap._M_start=data;map->SLONGMap._M_finish=data+entries;map->SLONGMap._M_end_of_storage._M_data=data?data+entries+3:0;
 for(int i=0;i<entries;++i){for(int j=0;j<3;++j){Ref* ref=alias==0?0:&refs[alias==1?0:(i*3+j)%8];*(Ref**)((char*)&data[i]+j*4)=ref;if(ref)++ref->count;}data[i].second.Val=i;for(int j=2;j>=0;--j)expected[expectedCount++]=(char*)&data[i]+j*4;}
 const void* prior[40];memcpy(prior,expected,sizeof(prior));
 for(int i=0;i<entries*3;++i)expected[i]=prior[count+i];
 for(int i=0;i<count;++i)expected[entries*3+i]=prior[i];
 map->SLONGMap.Dirty=true;map->CheckForDuplicateSymbols=alias==1;*(unsigned char*)&map->SLONGMap.Compare=0x7a;
 map->Clear();
 if(destroyCalls!=expectedCount||map->SLONGMap._M_start||map->SLONGMap._M_finish||map->SLONGMap._M_end_of_storage._M_data||map->SLONGMap.Dirty||*(unsigned char*)&map->SLONGMap.Compare!=0x7a||map->CheckForDuplicateSymbols!=(alias==1))return 5;
 if(map->NamesUsed._M_t._M_header._M_data!=header||map->NamesUsed._M_t._M_node_count||header->_M_parent||header->_M_left!=header||header->_M_right!=header)return 6;
 map->Clear();if(destroyCalls!=expectedCount)return 7;
 if(mode)delete map;else{CBaseClass* base=map;base->~CBaseClass();}
 if(deleteCalls!=mode||destroyCalls!=expectedCount)return 2;
 for(int i=0;i<expectedCount;++i)if(order[i]!=expected[i])return 3;
 for(int r=0;r<8;++r)if(refs[r].count!=1)return 4;
 owner=0;if(!mode)::operator delete(map);++cases;
}
printf("SYMBOL_MAP_CLEAR_BEHAVIOR_PASS scenarios=%d\n",cases);return 0;}
