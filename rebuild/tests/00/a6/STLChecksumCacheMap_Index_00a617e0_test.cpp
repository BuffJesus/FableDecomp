#include "fable_checksum_cache_file.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef _STL::map<CWideString,CChecksumCacheFile::CEntry> Map;
typedef Map::_Rep_type Tree;
typedef _STL::_Rb_tree_node<Map::value_type> Node;
static char events[32];static unsigned count;static Map* cacheMap;
static Node *bound,*inserted;static const CWideString* expectedKey;static Node* expectedHint;
static void record(char c){events[count++]=c;events[count]=0;}
static void require(bool v){if(!v)abort();}
CDateAndTime::CDateAndTime(){memset(this,0,sizeof(*this));record('D');}
CWideString::CWideString(const CWideString& other):storage_(other.storage_){record('C');}
CWideString::~CWideString(){record('X');}
bool CWideString::operator<(const CWideString& other) const {record('L');return wcscmp(reinterpret_cast<const wchar_t*>(storage_),reinterpret_cast<const wchar_t*>(other.storage_))<0;}
template<> Tree::_Link_type Tree::_M_lower_bound(const CWideString& key) const{require(this==&cacheMap->_M_t && &key==expectedKey);record('B');return bound;}
template<> Tree::iterator Tree::insert_unique(iterator hint,const Map::value_type& value){
    require(this==&cacheMap->_M_t && hint._M_node==expectedHint && *reinterpret_cast<void* const*>(&value.first)==*reinterpret_cast<void* const*>(expectedKey));
    const unsigned long* fields=reinterpret_cast<const unsigned long*>(&value.second.TimeStamp);for(unsigned i=0;i<7;++i)require(fields[i]==0);
    record('I');return iterator(inserted);
}
int main(){
    unsigned char rawMap[sizeof(Map)]={0},rawNodes[3][sizeof(Node)]={0},rawKey[sizeof(CWideString)]={0};
    cacheMap=reinterpret_cast<Map*>(rawMap);Node* header=reinterpret_cast<Node*>(rawNodes[0]);Node* existing=reinterpret_cast<Node*>(rawNodes[1]);inserted=reinterpret_cast<Node*>(rawNodes[2]);
    cacheMap->_M_t._M_header._M_data=header;CWideString& key=*reinterpret_cast<CWideString*>(rawKey);expectedKey=&key;
    *reinterpret_cast<const wchar_t**>(const_cast<CWideString*>(&existing->_M_value_field.first))=L"middle";
    for(unsigned mode=0;mode<3;++mode){
        count=0;events[0]=0;bound=mode==2?header:existing;expectedHint=bound;
        *reinterpret_cast<const wchar_t**>(&key)=mode==0?L"middle":mode==1?L"early":L"last";
        CChecksumCacheFile::CEntry& value=(*cacheMap)[key];
        if(&value!=(mode==0?&existing->_M_value_field.second:&inserted->_M_value_field.second))return 1;
        if(strcmp(events,mode==0?"BL":mode==1?"BLDCIX":"BDCIX"))return 2;
    }
    puts("CACHE_MAP_INDEX_00a617e0_PASS");return 0;
}
