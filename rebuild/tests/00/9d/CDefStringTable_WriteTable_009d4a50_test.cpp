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
int main(){unsigned lengths[]={0,4,32};for(unsigned k=0;k<3;++k){GDefStringTable.RandomID=0xabcdef01;GDefStringTable.StringCount=7;GDefStringTable.StringStream.Clear();unsigned char payload[32];for(unsigned i=0;i<32;++i)payload[i]=i+1;GDefStringTable.StringStream.Write(payload,lengths[k]);CMemoryDataOutputStream output;GDefStringTable.WriteTable(output);unsigned long header[]={0x7ab1e,0xabcdef01,7,lengths[k]};if(output.GetLength()!=16+lengths[k]||memcmp(output.PeekData(),header,16)||memcmp((const char*)output.PeekData()+16,payload,lengths[k]))return 1;}
 puts("DEFSTRING_WRITE_TABLE_OK cases=3");return 0;}
