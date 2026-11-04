#include "fable_memory_allocator.h"
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
CWideString::CWideString():storage_(0){}CWideString::~CWideString(){}
CDefStringTable::CDefStringTable():StringCount(0),SharedCount(0),RandomID(0xabcdef01),Locked(false),ReadOnly(false){}CDefStringTable::~CDefStringTable(){}
CDefStringTable GDefStringTable;static unsigned tableCalls;void CDefStringTable::SaveTable(bool text){if(text)abort();++tableCalls;}
static unsigned statsCalls;static char stats[4];CMemoryAllocator* g_FableMemoryAllocator_013CAA90=0;unsigned long CMemoryAllocator::GetTotalMemoryAllocated(){stats[statsCalls++]='A';return 0;}unsigned long CMemoryAllocator::GetNoAllocatedAreas(){stats[statsCalls++]='N';return 0;}
void CDataOutputStream::WriteEBOOL(bool value){unsigned char v=value;Write(&v,1);}void CDataOutputStream::WriteULONG(unsigned long value){Write(&value,4);}void CDataOutputStream::WriteSLONG(long value){Write(&value,4);}
template<>void CArrayVector<unsigned char>::DeallocateUnusedMemory(){}template<>void CVectorMap<unsigned long,unsigned long,CKeyPairCompareLess<unsigned long,unsigned long> >::DeallocateUnusedMemory(){}
template<>void FABLE_FASTCALL _STL::sort(_STL::pair<unsigned long,unsigned long>* first,_STL::pair<unsigned long,unsigned long>* last,CKeyPairCompareLess<unsigned long,unsigned long>){for(_STL::pair<unsigned long,unsigned long>* i=first;i!=last;++i)for(_STL::pair<unsigned long,unsigned long>* j=i+1;j<last;++j)if(j->first<i->first){_STL::pair<unsigned long,unsigned long> t=*i;*i=*j;*j=t;}}
static unsigned blockCalls;static unsigned firstIndices[8],blockCounts[8],blockSizes[8],blockOffsets[8][3];
void CDefinitionManager::CompressDefinitionBlock(CMemoryDataOutputStream& data,CArrayVector<unsigned short>& offsets,unsigned long first){for(unsigned i=0;i<offsets.size();++i)blockOffsets[blockCalls][i]=offsets[i];firstIndices[blockCalls]=first;blockCounts[blockCalls]=offsets.size();blockSizes[blockCalls]=data.GetLength();unsigned char output=90+blockCalls;unsigned long position=CompressedDefData.size();CompressedDefData.push_back(output);CompressedOffsets.Insert(first,position);++blockCalls;data.Clear();offsets.clear();}
struct FakeDef{unsigned long vptr,refs,fields[6],index;void Binary(CDataOutputStream& output);void Transfer(CPersistContext& context);};static unsigned payloadSizes[3];
void FakeDef::Binary(CDataOutputStream& output){unsigned char* buffer=(unsigned char*)malloc(payloadSizes[index]+1);memset(buffer,index+1,payloadSizes[index]);output.Write(buffer,payloadSizes[index]);free(buffer);}void FakeDef::Transfer(CPersistContext& context){if(context.Mode!=CPersistContext::MODE_BINARY_SAVE||!context.PSaveStream)abort();}
struct View:CDefinitionManager{using CDefinitionManager::InstantiatedDefs;using CDefinitionManager::CompressedDefData;using CDefinitionManager::CompressedOffsets;using CDefinitionManager::DependencyCheckCRC;using CDefinitionManager::UseSafeBinaryFormat;};
// RETAIL_EXPECTATIONS
int main(){unsigned counts[]={0,1,2,2,2,1,3};unsigned lengths[7][3]={{0},{5},{5,5},{16382,16382},{16383,16382},{40000},{32766,0,0}};unsigned long vtable[25]={0};void(FakeDef::*binary)(CDataOutputStream&)=&FakeDef::Binary;void(FakeDef::*transfer)(CPersistContext&)=&FakeDef::Transfer;memcpy(vtable+21,&binary,4);memcpy(vtable+18,&transfer,4);
 for(unsigned k=0;k<7;++k){unsigned long raw[52]={0};View* manager=(View*)raw;manager->DependencyCheckCRC=0x12345678;manager->UseSafeBinaryFormat=true;manager->CompressedDefData.push_back(99);manager->CompressedOffsets.Insert(100,100);CDefinitionManager::CInstantiatedDefInfo entries[3];FakeDef definitions[3];for(unsigned i=0;i<counts[k];++i){payloadSizes[i]=lengths[k][i];memset(definitions+i,0,sizeof(FakeDef));definitions[i].vptr=(unsigned long)vtable;definitions[i].refs=2;definitions[i].index=i;entries[i].ClassName.TablePos=10+i;entries[i].InstantiationName.TablePos=20+i;entries[i].ClassIndex=30+i;entries[i].PDef.Object=definitions+i;manager->InstantiatedDefs.push_back(entries+i);}CMemoryDataOutputStream output;blockCalls=tableCalls=statsCalls=0;g_FableMemoryAllocator_013CAA90=k==6?(CMemoryAllocator*)1:0;manager->SaveBinaryDefinitions(output);
 if(tableCalls!=1||statsCalls!=(k==6?4:0)||(k==6&&memcmp(stats,"ANAN",4)))return 1;for(unsigned i=0;i<counts[k];++i)if(entries[i].PDef.Object||definitions[i].refs!=1)return 2;
 // RETAIL_COMPARE
 manager->InstantiatedDefs._M_end_of_storage.deallocate(manager->InstantiatedDefs._M_start,0);manager->CompressedDefData._M_end_of_storage.deallocate(manager->CompressedDefData._M_start,0);manager->CompressedOffsets._M_end_of_storage.deallocate(manager->CompressedOffsets._M_start,0);}
 puts("DEFINITION_SAVE_OK cases=7");return 0;}
