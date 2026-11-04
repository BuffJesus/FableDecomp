#include "fable_def_string.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>

typedef _STL::map<CCharString,long> Map;
typedef Map::_Rep_type Tree;
typedef _STL::_Rb_tree_node<Map::value_type> Node;
char g_FableEmptyNarrowString_0129AAF4[1]={0};
static unsigned liveStrings,reads;
static unsigned char payload[4096];static unsigned payloadLength;
static CCharStringData* data(const CCharString& s){return *reinterpret_cast<CCharStringData* const*>(&s);}
static const char* text(const CCharString& s){return data(s)?data(s)->Data.PData:"";}
CCharString::CCharString():storage_(0){}
CCharString::CCharString(const char* value,long length){
    if(length<0)length=strlen(value);
    storage_=(CCharStringData*)calloc(1,sizeof(CCharStringData));
    storage_->Data.PData=(char*)malloc(length+1);memcpy(storage_->Data.PData,value,length);storage_->Data.PData[length]=0;
    storage_->Data.StringLength=length;++liveStrings;
}
CCharString::CCharString(const CCharString& other){
    storage_=0;
    if(data(other)){unsigned n=other.GetLength();storage_=(CCharStringData*)calloc(1,sizeof(CCharStringData));storage_->Data.PData=(char*)malloc(n+1);memcpy(storage_->Data.PData,text(other),n+1);storage_->Data.StringLength=n;++liveStrings;}
}
CCharString::~CCharString(){if(storage_){free(storage_->Data.PData);free(storage_);--liveStrings;}}
const void* CMemoryDataOutputStream::PeekData()const{++reads;return payload;}
unsigned long CMemoryDataOutputStream::GetLength()const{++reads;return payloadLength;}
static unsigned checkTree(_STL::_Rb_tree_node_base* base,_STL::_Rb_tree_node_base* parent,const char* lower,const char* upper,unsigned& count){
    if(!base)return 1;
    Node* node=static_cast<Node*>(base);const char* key=text(node->_M_value_field.first);
    if(base->_M_parent!=parent||(lower&&strcmp(lower,key)>=0)||(upper&&strcmp(key,upper)>=0))abort();
    if(base->_M_color==_STL::_S_rb_tree_red&&((base->_M_left&&base->_M_left->_M_color==_STL::_S_rb_tree_red)||(base->_M_right&&base->_M_right->_M_color==_STL::_S_rb_tree_red)))abort();
    ++count;unsigned left=checkTree(base->_M_left,base,lower,key,count),right=checkTree(base->_M_right,base,key,upper,count);
    if(left!=right)abort();return left+(base->_M_color==_STL::_S_rb_tree_black);
}
static void run(const char** names,unsigned n,bool locked,bool readonly){
    unsigned char storage[sizeof(CDefStringTable)];memset(storage,0,sizeof(storage));CDefStringTable& table=*reinterpret_cast<CDefStringTable*>(storage);
    new(&table.StringMap)Map;table.Locked=locked;table.ReadOnly=readonly;table.StringCount=n;
    payloadLength=4;memset(payload,0,sizeof(payload));unsigned offsets[64];
    for(unsigned i=0;i<n;++i){offsets[i]=payloadLength;unsigned crc=0x12345678+i;memcpy(payload+payloadLength,&crc,4);payloadLength+=4;unsigned bytes=strlen(names[i])+1;memcpy(payload+payloadLength,names[i],bytes);payloadLength+=bytes;}
    reads=0;table.Unlock();bool active=locked&&!readonly;unsigned unique=0;
    if(active){for(unsigned i=0;i<n;++i){bool first=true;unsigned last=i;for(unsigned j=0;j<i;++j)if(!strcmp(names[j],names[i]))first=false;for(unsigned j=i+1;j<n;++j)if(!strcmp(names[j],names[i]))last=j;if(first)++unique;
        CCharString key(names[i]);Node* found=static_cast<Node*>(table.StringMap._M_t._M_lower_bound(key));if(found==table.StringMap._M_t._M_header._M_data||strcmp(text(found->_M_value_field.first),names[i])||found->_M_value_field.second!=(long)offsets[last])abort();}
        if(reads!=2||table.Locked)abort();}
    else if(reads||table.Locked!=locked)abort();
    if(table.ReadOnly!=readonly||table.StringCount!=n||table.StringMap._M_t.size()!=unique||liveStrings!=unique)abort();
    _STL::_Rb_tree_node_base* header=table.StringMap._M_t._M_header._M_data;
    if(unique){unsigned nodes=0;if(header->_M_parent->_M_color!=_STL::_S_rb_tree_black)abort();checkTree(header->_M_parent,header,0,0,nodes);if(nodes!=unique)abort();
        _STL::_Rb_tree_node_base* left=header->_M_parent;while(left->_M_left)left=left->_M_left;
        _STL::_Rb_tree_node_base* right=header->_M_parent;while(right->_M_right)right=right->_M_right;
        if(header->_M_left!=left||header->_M_right!=right)abort();}
    table.StringMap.clear();if(liveStrings)abort();free(header);
}
int main(){
    const char* names[]={"alpha","","alpha","zeta","beta","gamma","zeta","aardvark"};
    for(unsigned n=0;n<=8;++n)run(names,n,true,false);
    run(names,8,false,false);run(names,8,false,true);run(names,8,true,true);
    const char* ascending[]={"a","b","c","d","e","f","g","h","i","j","k","l"};
    const char* descending[]={"l","k","j","i","h","g","f","e","d","c","b","a"};
    run(ascending,12,true,false);run(descending,12,true,false);
    puts("DEFSTRING_UNLOCK_COMPOSED_OK cases=14");return 0;
}
