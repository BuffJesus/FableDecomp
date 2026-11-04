#pragma once
#include "fable_checksum_cache_file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../src/compiled/00/99/CWideString_Less_0099af70.cpp"
#include "../../src/compiled/00/a6/STLChecksumCacheTree_erase_00a60a00.cpp"
#include "../../src/compiled/00/a6/STLChecksumCacheTree_LowerBound_00a609c0.cpp"
#include "../../src/compiled/00/a6/STLChecksumCacheMap_Index_00a617e0.cpp"
wchar_t g_FableEmptyWideString_0129A8E0[1]={0};
static unsigned liveStrings,errors;
static void require(bool v){if(!v)++errors;}
CDateAndTime::CDateAndTime(){memset(this,0,sizeof(*this));}
CWideString::CWideString():storage_(0){}
CWideString::CWideString(const wchar_t* text){
    storage_=static_cast<CWideStringData*>(malloc(sizeof(CWideStringData)));
    storage_->text=const_cast<wchar_t*>(text);storage_->unknown04=reinterpret_cast<unsigned long>(text+wcslen(text));storage_->unknown08=storage_->unknown04;storage_->owners=1;++liveStrings;
}
CWideString::CWideString(const CWideString& other):storage_(other.storage_){if(storage_)++storage_->owners;}
CWideString::~CWideString(){if(storage_ && --storage_->owners==0){free(storage_);--liveStrings;}}
typedef _STL::map<CWideString,CChecksumCacheFile::CEntry> Map;
typedef Map::_Rep_type Tree;
typedef _STL::_Rb_tree_node<Map::value_type> Node;
static const wchar_t* text(const CWideString& value){const CWideStringData* p=*reinterpret_cast<const CWideStringData* const*>(&value);return p?p->text:L"";}
static unsigned walked;static const wchar_t* previous;
static unsigned walk(_STL::_Rb_tree_node_base* node,_STL::_Rb_tree_node_base* parent){
    if(!node)return 1;
    require(node->_M_parent==parent);
    if(node->_M_color==_STL::_S_rb_tree_red){require(!node->_M_left || node->_M_left->_M_color==_STL::_S_rb_tree_black);require(!node->_M_right || node->_M_right->_M_color==_STL::_S_rb_tree_black);}
    unsigned left=walk(node->_M_left,node);const wchar_t* key=text(static_cast<Node*>(node)->_M_value_field.first);
    if(previous)require(wcscmp(previous,key)<0);previous=key;++walked;
    unsigned right=walk(node->_M_right,node);require(left==right);
    return left+(node->_M_color==_STL::_S_rb_tree_black?1:0);
}
static void verify(Tree& tree,unsigned size){
    _STL::_Rb_tree_node_base* h=tree._M_header._M_data;walked=0;previous=0;
    require(tree._M_node_count==size && h->_M_color==_STL::_S_rb_tree_red);
    if(!size){require(!h->_M_parent && h->_M_left==h && h->_M_right==h);return;}
    require(h->_M_parent->_M_color==_STL::_S_rb_tree_black);walk(h->_M_parent,h);require(walked==size);
    _STL::_Rb_tree_node_base* lo=h->_M_parent;while(lo->_M_left)lo=lo->_M_left;
    _STL::_Rb_tree_node_base* hi=h->_M_parent;while(hi->_M_right)hi=hi->_M_right;
    require(h->_M_left==lo && h->_M_right==hi);
    Map::iterator forward(lo);for(unsigned i=0;i<size;++i){require(forward._M_node!=h);require(&(++forward)==&forward);}require(forward._M_node==h);
    Map::iterator backward(h);for(unsigned j=0;j<size;++j){require(&(--backward)==&backward && backward._M_node!=h);}require(backward._M_node==lo);
}
static int RunInsertionFixture(){
    const wchar_t* keys[16]={L"00",L"01",L"02",L"03",L"04",L"05",L"06",L"07",L"08",L"09",L"10",L"11",L"12",L"13",L"14",L"15"};
    unsigned order[18]={7,3,11,1,5,9,13,0,2,4,6,8,10,12,14,7,0,14};
    for(unsigned mode=0;mode<5;++mode){
        {
            FableBankOwnedMap<CWideString,CChecksumCacheFile::CEntry> map;
            bool seen[16]={false};unsigned size=0;verify(map._M_t,size);
            for(unsigned step=0;step<18;++step){
                unsigned k=mode==4?(14-order[step]):order[step];CWideString key(keys[k]);CChecksumCacheFile::CEntry entry;entry.TimeStamp.Year=2000+k;entry.Checksum=seen[k]?0xDEADBEEF:0x9000+k;
                Map::value_type value(key,entry);Map::iterator result;
                if(!mode){_STL::pair<Map::iterator,bool> inserted=map._M_t.insert_unique(value);require(inserted.second==!seen[k]);result=inserted.first;}
                else {
                    _STL::_Rb_tree_node_base* h=map._M_t._M_header._M_data;
                    _STL::_Rb_tree_node_base* hint=mode==1?h:mode==2?h->_M_left:mode==3?h->_M_right:(h->_M_parent?h->_M_parent:h);
                    result=map._M_t.insert_unique(Map::iterator(hint),value);
                }
                require(!wcscmp(text(result->first),keys[k]) && result->second.Checksum==0x9000+k && result->second.TimeStamp.Year==2000+k);
                if(!seen[k]){seen[k]=true;++size;}verify(map._M_t,size);
            }
            {CWideString key(keys[15]);CChecksumCacheFile::CEntry& value=map[key];require(value.TimeStamp.Year==0);value.Checksum=0x900F;verify(map._M_t,16);require(&map[key]==&value && value.Checksum==0x900F);}
        }
        require(liveStrings==0);
    }
    return errors?1:0;
}
