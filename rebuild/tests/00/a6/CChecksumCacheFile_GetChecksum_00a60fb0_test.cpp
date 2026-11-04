#include "../src/compiled/00/99/CWideString_Less_0099af70.cpp"
#include "../src/compiled/00/a6/STLChecksumCacheTree_erase_00a60a00.cpp"
#include "../src/compiled/00/a6/STLChecksumCacheTree_LowerBound_00a609c0.cpp"
#include "../src/compiled/00/a6/STLChecksumCacheMap_Index_00a617e0.cpp"
#include "../src/compiled/00/a6/STLChecksumCacheTree_InsertNode_00a61270.cpp"
#include "../src/compiled/00/a6/STLChecksumCacheTree_InsertUnique_00a61360.cpp"
#include "../src/compiled/00/a6/STLChecksumCacheTree_InsertHint_00a614b0.cpp"
#include "../src/compiled/00/a6/CChecksumCacheFile_CChecksumCacheFile_00a60c90.cpp"
#include "../src/compiled/00/a6/CChecksumCacheFile_Close_00a60c30.cpp"
#include "../src/compiled/00/a6/CChecksumCacheFile_Destructor_00a60ce0.cpp"
#include "../src/compiled/00/40/Global_CCRC_Calc_004014a0.cpp"
#include "../src/compiled/00/a6/STLChecksumCacheTree_Find_00a608f0.cpp"
#include "../src/compiled/00/9e/CDateAndTime_Equal_009e9ad0.cpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include "fable_checksum_cache_file.h"
#include "fable_disk_file.h"
#include "fable_crc.h"
class CFileInformation {public: unsigned char Bytes[0x58];};
static CChecksumCacheFile* cache;
static unsigned reads,modifiedQueries;
static long fileLength;
static unsigned char fileBytes[80];
static long stampYear;
fable_u32 g_FableCRCTable_0129A168[256];
CDateAndTime __fastcall CAFile::GetFileModifiedTime(const CWideString& name){(void)name; CDateAndTime time;time.Year=stampYear;++modifiedQueries;return time;}
static unsigned liveWide,liveNarrow;
wchar_t g_FableEmptyWideString_0129A8E0[1]={0};
char g_FableEmptyNarrowString_0129AAF4[1]={0};
static const CWideString* inputName;
static unsigned opens,closes,existsCalls,streamDtors,erases,inserts,overflows,finalPosition;
static bool exists,overflowMode;
static unsigned char blob[128];static unsigned blobSize;
static void require(bool value) {if(!value){puts("cache checksum fixture mismatch");exit(1);}}
CBaseClass::CBaseClass() {} CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {} CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CWideString::CWideString():storage_(0){}
CWideString::CWideString(const wchar_t* value) {
    unsigned n=wcslen(value);storage_=static_cast<CWideStringData*>(malloc(sizeof(CWideStringData)));
    wchar_t* data=static_cast<wchar_t*>(malloc((n+1)*2));memcpy(data,value,(n+1)*2);
    storage_->text=data;storage_->unknown04=reinterpret_cast<unsigned long>(data+n);storage_->unknown08=reinterpret_cast<unsigned long>(data+n+1);storage_->owners=1;++liveWide;
}
CWideString::CWideString(const CWideString& value):storage_(value.storage_){if(storage_)++storage_->owners;}
CWideString::~CWideString(){if(storage_ && --storage_->owners==0){free(storage_->text);free(storage_);--liveWide;}}
CWideString& CWideString::operator=(const CWideString& other){if(this!=&other){this->~CWideString();storage_=other.storage_;if(storage_)++storage_->owners;}return *this;}
CCharString::CCharString():storage_(0){}
CCharString::CCharString(const char* text,long length){
    if(length<0)length=strlen(text);storage_=static_cast<CCharStringData*>(malloc(sizeof(CCharStringData)));memset(storage_,0,sizeof(*storage_));
    storage_->text=static_cast<char*>(malloc(length+1));memcpy(storage_->text,text,length);storage_->text[length]=0;
    storage_->Data.StringLength=length;storage_->unknown08=length+1;storage_->owners=1;++liveNarrow;
}
CCharString::CCharString(const CCharString& other):storage_(other.storage_){if(storage_)++storage_->owners;}
CCharString::~CCharString(){if(storage_ && --storage_->owners==0){free(storage_->text);free(storage_);--liveNarrow;}}
CWideString CCharString::ToWideString() const {
    unsigned n=storage_?storage_->Data.StringLength:0;wchar_t* text=static_cast<wchar_t*>(malloc((n+1)*2));
    for(unsigned i=0;i<n;++i)text[i]=static_cast<unsigned char>(storage_->text[i]);text[n]=0;
    CWideString result(text);free(text);return result;
}
CDateAndTime::CDateAndTime(){memset(this,0,sizeof(*this));}
CFileInformation CAFile::GetFileInformation() { CFileInformation value; memset(&value, 0, sizeof(value)); return value; }
bool __fastcall CAFile::PathExists(const CWideString&) { ++existsCalls; return exists; }
void CDiskFileWin32::Open(const CWideString& path,FileAccessType access,unsigned long flags){
    require(flags==2 && !Opened);
    if(access==FILE_ACCESS_CREATE){require(&path==&cache->Filename);blobSize=0;}
    else require(access==FILE_ACCESS_READ && &path==inputName);
    AccessType=access;Position=0;Length=fileLength;Opened=true;++opens;
}
void CDiskFileWin32::Close() { Opened = false; ++closes; }
void CDiskFileWin32::Read(void* data,long size,CASuspendableProcess* process){
 require(Opened && AccessType==FILE_ACCESS_READ && process==0 && size>0 && size<=cache->BufferSize && Position+size<=fileLength);
 memcpy(data,fileBytes+Position,size);Position+=size;++reads;
}
void CDiskFileWin32::Write(const void* data,long size,CASuspendableProcess* process){
    require(Opened && AccessType==FILE_ACCESS_CREATE && process==0 && size>=0 && Position>=0 && Position+size<=sizeof(blob));
    memcpy(blob+Position,data,size);Position+=size;if(blobSize<static_cast<unsigned>(Position))blobSize=Position;
}
void CDiskFileWin32::SetPosition(long position) {require(position>=0);Position=position;}
void CDiskFileWin32::MovePosition(long) { abort(); }
long CDiskFileWin32::GetPosition() const {return Position;}
void CDiskFileWin32::SetLength(long) { abort(); }
CAFile* CDiskFileWin32::Clone() const { abort(); return 0; }
CFileInformation CDiskFileWin32::GetFileInformation() { CFileInformation value; memset(&value, 0, sizeof(value)); return value; }

typedef _STL::map<CWideString,CChecksumCacheFile::CEntry> CacheMap;
typedef CacheMap::_Rep_type CacheTree;
typedef _STL::_Rb_tree_node<CacheMap::value_type> CacheNode;
static const wchar_t* keyText(const CWideString& value){const CWideStringData* data=*reinterpret_cast<const CWideStringData* const*>(&value);return data?data->text:L"";}
static const CChecksumCacheFile::CEntry* lookup(const wchar_t* text){CWideString key(text);_STL::_Rb_tree_node_base* n=cache->Entries._M_t._M_lower_bound(key);if(n==cache->Entries._M_t._M_header._M_data)return 0;CacheNode* node=static_cast<CacheNode*>(n);return wcscmp(keyText(node->_M_value_field.first),text)?0:&node->_M_value_field.second;}
const CWideString& CWideString::operator=(const wchar_t* value){CWideString temp(value);*this=temp;return *this;}

long CWideString::GetLength() const{return storage_?wcslen(storage_->text):0;}
void CChecksumCacheFile::Save(){abort();}
static unsigned long referenceCRC(const unsigned char* bytes,unsigned length){
 unsigned long crc=0xffffffff;
 for(unsigned i=0;i<length;++i){crc^=bytes[i];for(unsigned bit=0;bit<8;++bit)crc=(crc>>1)^((crc&1)?0xedb88320:0);}
 return ~crc;
}
int main(){
 for(unsigned i=0;i<256;++i){unsigned long crc=i;for(unsigned bit=0;bit<8;++bit)crc=(crc>>1)^((crc&1)?0xedb88320:0);g_FableCRCTable_0129A168[i]=crc;}
 for(unsigned n=0;n<sizeof(fileBytes);++n)fileBytes[n]=static_cast<unsigned char>(n*37+129);
 for(unsigned mode=0;mode<5;++mode){
  unsigned lengths[5]={0,1,16,17,63};fileLength=lengths[mode];
  unsigned char raw[sizeof(CChecksumCacheFile)];cache=new(raw) CChecksumCacheFile(8);
  require(cache->BufferSize==16);
  CWideString name(L"asset.bin");inputName=&name;
  exists=false;stampYear=2000;reads=opens=closes=modifiedQueries=0;
  require(cache->GetChecksum(name)==0 && !opens && !reads && !modifiedQueries && !cache->NeedsSaving && !cache->Entries._M_t._M_node_count);
  exists=true;unsigned long expected=referenceCRC(fileBytes,fileLength);
  require(cache->GetChecksum(name)==expected && opens==1 && closes==1 && modifiedQueries==1 && reads==(fileLength+15)/16 && cache->NeedsSaving && cache->Entries._M_t._M_node_count==1);
  require(lookup(L"asset.bin")->TimeStamp.Year==2000);
  cache->NeedsSaving=false;unsigned previousReads=reads;
  require(cache->GetChecksum(name)==expected && opens==1 && closes==1 && reads==previousReads && modifiedQueries==2 && !cache->NeedsSaving);
  if(fileLength)fileBytes[0]^=0x80;
  stampYear=2001;expected=referenceCRC(fileBytes,fileLength);
  require(cache->GetChecksum(name)==expected && opens==2 && closes==2 && modifiedQueries==3 && reads==previousReads*2 && cache->NeedsSaving && cache->Entries._M_t._M_node_count==1);
  require(lookup(L"asset.bin")->TimeStamp.Year==2001);
  cache->NeedsSaving=false;exists=false;
  require(cache->GetChecksum(name)==0 && opens==2 && modifiedQueries==3 && !cache->NeedsSaving && cache->Entries._M_t._M_node_count==1);
  cache->~CChecksumCacheFile();require(liveWide==1 && !liveNarrow);
 }
 require(!liveWide && !liveNarrow);puts("CACHE_GET_CHECKSUM_00a60fb0_PASS");return 0;
}
