#include "fable_checksum_cache_file.h"
#include <stdio.h>
#include <stdlib.h>
#include <new>
#include <string.h>
static CChecksumCacheFile* expected;static const CWideString* filename;static unsigned calls;static long bufferSize;
static void require(bool v){if(!v)abort();}
CWideString::CWideString():storage_(0){} CWideString::~CWideString(){}
CChecksumCacheFile::~CChecksumCacheFile(){}
typedef _STL::map<CWideString,CChecksumCacheFile::CEntry>::_Rep_type Tree;
template<> void Tree::_M_erase(_STL::_Rb_tree_node<_STL::map<CWideString,CChecksumCacheFile::CEntry>::value_type>*){abort();}
void CChecksumCacheFile::Open(const CWideString& name){
 require(this==expected && &name==filename && BufferSize==bufferSize && !NeedsSaving && !Entries._M_t._M_node_count);
 _STL::_Rb_tree_node_base* h=Entries._M_t._M_header._M_data;require(h && !h->_M_parent && h->_M_left==h && h->_M_right==h);
 require(*reinterpret_cast<void**>(&Filename)==0);++calls;
}
int main(){
 CWideString name;filename=&name;long sizes[5]={0,1,8,16,0x4000};
 for(unsigned i=0;i<5;++i){unsigned char raw[sizeof(CChecksumCacheFile)+8];memset(raw,0xA5,sizeof(raw));expected=reinterpret_cast<CChecksumCacheFile*>(raw+4);bufferSize=(sizes[i]+8)&~8;
  CChecksumCacheFile* value=new(raw+4)CChecksumCacheFile(name,sizes[i]);require(value==expected && calls==i+1 && raw[3]==0xA5 && raw[sizeof(raw)-4]==0xA5);value->~CChecksumCacheFile();
 }
 puts("CHECKSUM_CACHE_OPEN_CTOR_00a61150_PASS");return 0;
}
