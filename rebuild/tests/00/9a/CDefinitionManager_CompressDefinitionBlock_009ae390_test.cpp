#include <new>
#include "fable_stlport_vector_impl.h"
#include "engine/CCompressorZlib.h"
#include "fable_definition_manager.h"
#include "fable_string_parser.h"
#include "fable_persist_context.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char g_FableEmptyNarrowString_0129AAF4[1]={0};
static CCharStringData*& slot(const CCharString& s){return *reinterpret_cast<CCharStringData**>(const_cast<CCharString*>(&s));}
static const char* text(const CCharString& s){return slot(s)?slot(s)->Data.PData:"";}
static CCharStringData* make(const char* s,long length){if(length<0)length=strlen(s);CCharStringData* d=(CCharStringData*)calloc(1,sizeof(CCharStringData));d->Data.PData=(char*)malloc(length+1);memcpy(d->Data.PData,s,length);d->Data.PData[length]=0;d->Data.StringLength=length;return d;}
static void release(CCharStringData* d){if(d){free(d->Data.PData);free(d);}}
CCharString::CCharString():storage_(0){}
CCharString::CCharString(const char* s,long length):storage_(make(s,length)){}
CCharString::CCharString(const CCharString& s):storage_(make(text(s),-1)){}
CCharString::~CCharString(){release(storage_);}
CCharString::operator const char*()const{return text(*this);}
CCharString& CCharString::operator=(const CCharString& s){CCharStringData* d=make(text(s),-1);release(storage_);storage_=d;return *this;}
const CCharString& CCharString::operator=(const char* s){CCharStringData* d=make(s,-1);release(storage_);storage_=d;return *this;}
void CCharString::SetAsSubString(const char* s,long length){CCharStringData* d=make(s,length);release(storage_);storage_=d;}
template<>bool CBasicString<char>::operator==(const char* s)const{return !strcmp(PData,s);}
CCharString FABLE_FASTCALL operator+(const CCharString& a,const CCharString& b){char s[256];strcpy(s,text(a));strcat(s,text(b));return CCharString(s);}
CCharString FABLE_FASTCALL operator+(const CCharString& a,const char* b){char s[256];strcpy(s,text(a));strcat(s,b);return CCharString(s);}

const CCharString& CCharString::operator+=(const char* suffix){char result[256];strcpy(result,text(*this));strcat(result,suffix);*this=result;return *this;}
CBaseClass::CBaseClass(){}CBaseClass::~CBaseClass(){}CBaseClassNonCopyable::CBaseClassNonCopyable(){}CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CMemoryDataOutputStream::CMemoryDataOutputStream():CurrentPosition(0){}CMemoryDataOutputStream::~CMemoryDataOutputStream(){}
void CMemoryDataOutputStream::SetPosition(unsigned long p){CurrentPosition=p;}unsigned long CMemoryDataOutputStream::GetPosition()const{return CurrentPosition;}void CMemoryDataOutputStream::Close(){}
void CMemoryDataOutputStream::Write(const void* input,long size){unsigned long end=CurrentPosition+size;if(end>Data.size())Data.resize(end,0);if(size)memcpy(Data._M_start+CurrentPosition,input,size);CurrentPosition=end;}
unsigned long CMemoryDataOutputStream::GetLength()const{return Data.size();}const void* CMemoryDataOutputStream::PeekData()const{return Data.GetAsCArray();}
CACompressor::CACompressor(long size):Workspace(0),WorkspaceSize(size),CompressionOn(true){}
void CACompressor::CompressAsImage(void*,unsigned long,void*,unsigned long,long,long,long,CCompressionInfo&){}void CACompressor::Compress(void*,unsigned long,void*,unsigned long,CCompressionInfo&){}void CACompressor::Decompress(void*,unsigned long,void*,unsigned long,CCompressionInfo&){}void CACompressor::SetQuality(float){}
unsigned long FABLE_FASTCALL CCompressorZlib::GetRequiredWorkspaceSize(){return 0;}void CCompressorZlib::Initialise(){}CCompressorZlib::~CCompressorZlib(){}
void CCompressorZlib::Compress(const void*,unsigned long,void*,unsigned long,CCompressionInfo&){}void CCompressorZlib::CompressAsImage(const void*,unsigned long,void*,unsigned long,long,long,long,CCompressionInfo&){}void CCompressorZlib::Decompress(const void*,unsigned long,void*,unsigned long,CCompressionInfo&){}
static unsigned char expected[64];static unsigned expectedSize,calls;
unsigned long CCompressorZlib::InternalCompress(const void* input,unsigned long size,void* output,unsigned long capacity){if(size!=expectedSize||capacity!=size+1024||memcmp(input,expected,size))abort();++calls;memcpy(output,"ABC",3);return 3;}
struct View:CDefinitionManager{using CDefinitionManager::CompressDefinitionBlock;using CDefinitionManager::CompressedDefData;using CDefinitionManager::CompressedOffsets;};
int main(){unsigned counts[]={0,1,3,2},sizes[]={0,5,15,5};for(unsigned k=0;k<4;++k){unsigned long raw[52]={0};View* manager=(View*)raw;manager->CompressedDefData.push_back(9);manager->CompressedDefData.push_back(8);CMemoryDataOutputStream data;unsigned char payload[15];for(unsigned i=0;i<15;++i)payload[i]=i+1;data.Write(payload,sizes[k]);CArrayVector<unsigned short> offsets;offsets.reserve(5);for(unsigned i=0;i<counts[k];++i){unsigned short offset=k==3?(i?1:65535):i*4;offsets.push_back(offset);unsigned short adjusted=(unsigned short)(offset+counts[k]*2);memcpy(expected+i*2,&adjusted,2);}memcpy(expected+counts[k]*2,payload,sizes[k]);expectedSize=counts[k]*2+sizes[k];calls=0;manager->CompressDefinitionBlock(data,offsets,17);
 if(calls!=1||data.GetLength()!=0||data.CurrentPosition||data.Data._M_start||offsets.size()!=0||offsets.capacity()!=5||manager->CompressedDefData.size()!=5||manager->CompressedDefData[0]!=9||manager->CompressedDefData[1]!=8||memcmp(manager->CompressedDefData._M_start+2,"ABC",3)||manager->CompressedOffsets.size()!=1||manager->CompressedOffsets[0].first!=17||manager->CompressedOffsets[0].second!=2||!manager->CompressedOffsets.Dirty)return 1;
 manager->CompressedDefData._M_end_of_storage.deallocate(manager->CompressedDefData._M_start,0);manager->CompressedOffsets._M_end_of_storage.deallocate(manager->CompressedOffsets._M_start,0);}
 CMemoryDataOutputStream reserved;reserved.Data.reserve(10);reserved.CurrentPosition=99;reserved.Clear();if(reserved.Data.capacity()||reserved.Data._M_start||reserved.CurrentPosition)return 2;
 puts("DEFINITION_COMPRESS_BLOCK_OK cases=4");return 0;}
