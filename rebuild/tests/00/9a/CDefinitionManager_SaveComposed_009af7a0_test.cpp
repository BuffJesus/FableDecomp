#include "fable_memory_allocator.h"
#include <new>
#include "fable_stlport_vector_impl.h"
#include "engine/CCompressorZlib.h"
#include "fable_definition_manager.h"
#include "fable_string_parser.h"
#include "fable_persist_context.h"
#include "fable_disk_file.h"
#include "fable_file_data_output_stream.h"
#include "test_support/data_input_stream_stubs.h"
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
// The native output implementation shares a TU with unused input constructors.
// These input-only slots must never run in this save fixture.
void CDataInputStream::SetPosition(unsigned long){abort();}
void CDataInputStream::ReadWithSrcChunkOverflow(void*,long){abort();}
CFileDataInputStream::~CFileDataInputStream(){abort();}
CMemoryDataInputStream::~CMemoryDataInputStream(){abort();}
CACompressor::CACompressor(long size):Workspace(0),WorkspaceSize(size),CompressionOn(true){}
void CACompressor::CompressAsImage(void*,unsigned long,void*,unsigned long,long,long,long,CCompressionInfo&){}void CACompressor::Compress(void*,unsigned long,void*,unsigned long,CCompressionInfo&){}void CACompressor::Decompress(void*,unsigned long,void*,unsigned long,CCompressionInfo&){}void CACompressor::SetQuality(float){}
unsigned long FABLE_FASTCALL CCompressorZlib::GetRequiredWorkspaceSize(){return 0;}void CCompressorZlib::Initialise(){}CCompressorZlib::~CCompressorZlib(){}
void CCompressorZlib::Compress(const void*,unsigned long,void*,unsigned long,CCompressionInfo&){}void CCompressorZlib::CompressAsImage(const void*,unsigned long,void*,unsigned long,long,long,long,CCompressionInfo&){}void CCompressorZlib::Decompress(const void*,unsigned long,void*,unsigned long,CCompressionInfo&){}
static unsigned char expected[64];static unsigned expectedSize,calls;

CWideString::CWideString():storage_(0){}CWideString::~CWideString(){}
CDefStringTable::CDefStringTable():StringCount(0),SharedCount(0),RandomID(0xabcdef01),Locked(false),ReadOnly(false){}CDefStringTable::~CDefStringTable(){}
CDefStringTable GDefStringTable;static unsigned tableCalls,closeCalls,fileLength;
static unsigned char fileBytes[128];
class CFileInformation {};
CAFile::CAFile(){}
CFileInformation CAFile::GetFileInformation(){return CFileInformation();}
CWideString::CWideString(const CWideString& other):storage_(other.storage_){}
void CDiskFileWin32::Open(const CWideString& path,FileAccessType mode,unsigned long flags){if(&path!=&GDefStringTable.TableFilename||mode!=FILE_ACCESS_CREATE||flags!=4||IsOpen())abort();Opened=true;fileLength=0;++tableCalls;}
void CDiskFileWin32::Close(){if(!IsOpen())abort();Opened=false;++closeCalls;}
void CDiskFileWin32::Read(void*,long,CASuspendableProcess*){}
void CDiskFileWin32::Write(const void* bytes,long size,CASuspendableProcess* process){if(!IsOpen()||process||size<0||fileLength+size>sizeof(fileBytes))abort();if(size)memcpy(fileBytes+fileLength,bytes,size);fileLength+=size;}
void CDiskFileWin32::SetPosition(long){}
void CDiskFileWin32::MovePosition(long){}
long CDiskFileWin32::GetPosition()const{return fileLength;}
void CDiskFileWin32::SetLength(long){}
CAFile* CDiskFileWin32::Clone()const{return 0;}
CFileInformation CDiskFileWin32::GetFileInformation(){return CFileInformation();}
struct _RTL_CRITICAL_SECTION { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(_RTL_CRITICAL_SECTION*);
static long Held() { return g_CMemoryAllocatorCriticalSection_013CAA70.RecursionCount; }

static unsigned statsCalls;static char stats[4];
static unsigned long Record(int, int slot) { if(Held()!=1 || statsCalls>=4) abort();stats[statsCalls++]=slot==0?'A':'N';return slot==0?0xfffffff0UL:7; }
void CMemoryAllocatorBase::ConsolidateFreeAreas() { abort(); }
class CRecordingAllocator : public CMemoryAllocatorBase
{
public:
    CRecordingAllocator() : CMemoryAllocatorBase(4) {}
    char Tag; long HeldDuringCall;
    void* Allocate(unsigned long) { return 0; }
    void Deallocate(void*) {}
    unsigned long GetAllAllocatedAreas(void**, unsigned long) { return 0; }
    unsigned long GetAllocationSize(void*) { return 0; }
    unsigned long GetAllocationSizeIncludingHeader(void*) { return 0; }
    unsigned long GetLargestFreeAreaSize() { return 0; }
    unsigned long GetTotalMemoryControlledByAllocator() { return 0; }
    unsigned long GetTotalMemoryAllocated() { return Record(Tag,0); }
    unsigned long GetNoFreeAreas() { return 0; }
    unsigned long GetNoAllocatedAreas() { return Record(Tag,1); }
    float GetFragmentation() { return 0; }
    void* GetAllocatedArea(long) { return 0; }
    unsigned long GetHeaderOverhead() { return 0; }
    void ConsolidateFreeAreas() { abort(); }
    void Validate() {}
};

struct Peek : public CMemoryAllocator
{
    static CAllocatorInfo* Info() { return AllocatorInfo; }
};

CMemoryAllocator* g_FableMemoryAllocator_013CAA90=0;

static unsigned blockCalls;static unsigned firstIndices[8],blockCounts[8],blockSizes[8],blockOffsets[8][3];
struct FakeDef{unsigned long vptr,refs,fields[6],index;void Binary(CDataOutputStream& output);void Transfer(CPersistContext& context);};static unsigned payloadSizes[3];
void FakeDef::Binary(CDataOutputStream& output){unsigned char* buffer=(unsigned char*)malloc(payloadSizes[index]+1);memset(buffer,index+1,payloadSizes[index]);output.Write(buffer,payloadSizes[index]);free(buffer);}void FakeDef::Transfer(CPersistContext& context){if(context.Mode!=CPersistContext::MODE_BINARY_SAVE||!context.PSaveStream)abort();}
struct View:CDefinitionManager{using CDefinitionManager::InstantiatedDefs;using CDefinitionManager::CompressedDefData;using CDefinitionManager::CompressedOffsets;using CDefinitionManager::DependencyCheckCRC;using CDefinitionManager::UseSafeBinaryFormat;};
// RETAIL_EXPECTATIONS
static unsigned currentCase;
unsigned long CCompressorZlib::InternalCompress(const void* input,unsigned long size,void* output,unsigned long capacity){unsigned count=expectedCounts[currentCase][blockCalls],first=expectedFirst[currentCase][blockCalls];if(size!=expectedPayloads[currentCase][blockCalls]+count*2||capacity!=size+1024)abort();const unsigned char* bytes=(const unsigned char*)input;for(unsigned i=0;i<count;++i){unsigned short offset;memcpy(&offset,bytes+i*2,2);if(offset!=(unsigned short)(expectedOffsets[currentCase][blockCalls][i]+count*2))abort();blockOffsets[blockCalls][i]=expectedOffsets[currentCase][blockCalls][i];}unsigned position=count*2;for(unsigned i=0;i<count;++i)for(unsigned j=0;j<payloadSizes[first+i];++j)if(bytes[position++]!=first+i+1)abort();firstIndices[blockCalls]=first;blockCounts[blockCalls]=count;blockSizes[blockCalls]=size-count*2;*(unsigned char*)output=90+blockCalls;++blockCalls;return 1;}

int main(){InitializeCriticalSection(&g_CMemoryAllocatorCriticalSection_013CAA70);CRecordingAllocator sizeClass;sizeClass.Tag=0;Peek::Info()[0].PAllocator=&sizeClass;static unsigned long allocatorStorage[0x420/4]={0};allocatorStorage[0x410/4]=1;unsigned counts[]={0,1,2,2,2,1,3};unsigned lengths[7][3]={{0},{5},{5,5},{16382,16382},{16383,16382},{40000},{32766,0,0}};unsigned long vtable[25]={0};void(FakeDef::*binary)(CDataOutputStream&)=&FakeDef::Binary;void(FakeDef::*transfer)(CPersistContext&)=&FakeDef::Transfer;memcpy(vtable+21,&binary,4);memcpy(vtable+18,&transfer,4);
 for(unsigned k=0;k<7;++k){unsigned long raw[52]={0};View* manager=(View*)raw;manager->DependencyCheckCRC=0x12345678;manager->UseSafeBinaryFormat=true;manager->CompressedDefData.push_back(99);manager->CompressedOffsets.Insert(100,100);CDefinitionManager::CInstantiatedDefInfo entries[3];FakeDef definitions[3];for(unsigned i=0;i<counts[k];++i){payloadSizes[i]=lengths[k][i];memset(definitions+i,0,sizeof(FakeDef));definitions[i].vptr=(unsigned long)vtable;definitions[i].refs=2;definitions[i].index=i;entries[i].ClassName.TablePos=10+i;entries[i].InstantiationName.TablePos=20+i;entries[i].ClassIndex=30+i;entries[i].PDef.Object=definitions+i;manager->InstantiatedDefs.push_back(entries+i);}GDefStringTable.StringStream.Clear();unsigned char tablePayload[32];for(unsigned q=0;q<32;++q)tablePayload[q]=q+1;unsigned tableLength=k%3==0?0:k%3==1?4:32;GDefStringTable.StringStream.Write(tablePayload,tableLength);GDefStringTable.StringCount=k;CMemoryDataOutputStream output;currentCase=k;blockCalls=tableCalls=statsCalls=0;g_FableMemoryAllocator_013CAA90=k==6?reinterpret_cast<CMemoryAllocator*>(allocatorStorage):0;manager->SaveBinaryDefinitions(output);
 if(Held()!=0||tableCalls!=1||statsCalls!=(k==6?4:0)||(k==6&&memcmp(stats,"ANAN",4)))return 1;for(unsigned i=0;i<counts[k];++i)if(entries[i].PDef.Object||definitions[i].refs!=1)return 2;
 {unsigned long header[]={0x7ab1e,GDefStringTable.RandomID,GDefStringTable.StringCount,GDefStringTable.StringStream.GetLength()};if(closeCalls!=k+1||fileLength!=16+header[3]||memcmp(fileBytes,header,16)||memcmp(fileBytes+16,GDefStringTable.StringStream.PeekData(),header[3])||GDefStringTable.Locked)return 7;}
 if(manager->CompressedDefData.capacity()!=manager->CompressedDefData.size()||manager->CompressedOffsets.capacity()!=manager->CompressedOffsets.size()||manager->CompressedOffsets.Dirty)return 8;
 // RETAIL_COMPARE
 manager->InstantiatedDefs._M_end_of_storage.deallocate(manager->InstantiatedDefs._M_start,0);manager->CompressedDefData._M_end_of_storage.deallocate(manager->CompressedDefData._M_start,0);manager->CompressedOffsets._M_end_of_storage.deallocate(manager->CompressedOffsets._M_start,0);}
 puts("DEFINITION_SAVE_OK cases=7");return 0;}
