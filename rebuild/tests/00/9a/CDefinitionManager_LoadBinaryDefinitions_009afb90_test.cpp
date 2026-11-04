#include <new>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fable_definition_manager.h"
#include "fable_data_input_stream.h"
#include "fable_progress_display.h"
typedef CDefinitionManager::CDefClassInfo Info;
typedef _STL::pair<unsigned long,Info> ClassEntry;
typedef _STL::pair<unsigned long,long> NameEntry;
typedef CVectorMap<unsigned long,Info,CKeyPairCompareLess<unsigned long,Info> > ClassMap;
static char calls[512]; static void log(char c){size_t n=strlen(calls);calls[n]=c;calls[n+1]=0;}
extern "C" unsigned char fakeTable[sizeof(CDefStringTable)]={};
#pragma comment(linker,"/alternatename:?GDefStringTable@@3VCDefStringTable@@A=_fakeTable")
CCharString::CCharString(const char* text,long len){if(strcmp(text,"Definition Manager: Load Binary Def") || len!=-1)abort();log('c');}
CCharString::~CCharString(){log('d');}
void FABLE_FASTCALL NProgressDisplay::DisplayProgress(const CCharString&,float f,bool force,bool debug){if(f!=-1.0f || force || debug)abort();log('p');}
void CDefString::TransferBinaryIn(CDataInputStream& s){log('t');s.Read(&TablePos,4);}
unsigned long CDefStringTable::GetStringCRC(long pos) const {log('h');return pos==10?100:pos==11?999:pos==20?300:pos==21?200:0;}
void CDataInputStream::ReadWithSrcChunkOverflow(void*,long){abort();}
template<> ClassEntry* ClassMap::LowerBound(const unsigned long& key) {log('l');return key<=100?this->begin():this->end();}
namespace _STL {
 template<> void FABLE_FASTCALL sort(NameEntry* first,NameEntry* last,CKeyPairCompareLess<unsigned long,long>) {
  log('s');for(NameEntry* p=first;p!=last;++p)for(NameEntry* q=p+1;q!=last;++q)if(q->first<p->first){NameEntry t=*p;*p=*q;*q=t;}
 }
}
struct Access : CDefinitionManager {
 using CDefinitionManager::DependencyCheckCRC; using CDefinitionManager::UseSafeBinaryFormat;
 using CDefinitionManager::DefClassInfo; using CDefinitionManager::InstantiatedDefs;
 using CDefinitionManager::NameCRCToIndexMap;using CDefinitionManager::CompressedOffsets;using CDefinitionManager::CompressedDefData;
};
void CDefinitionManager::ClearInstantiatedDefs(){log('x');InstantiatedDefs._M_finish=InstantiatedDefs._M_start;}
static unsigned char input[128];static int len;
static void byte(unsigned char n){input[len++]=n;}
static void word(unsigned long n){memcpy(input+len,&n,4);len+=4;}
static void packet(unsigned long crc,unsigned long random,int count) {
 len=0;byte(1);word(crc);word(random);word(count);
 for(int i=0;i<count;++i){word(i==1?11:10);word(i==2?21:20);word(7+i);}
 word(count?2:0);if(count){word(0);word(3);word(3);word(2);}
 word(count?5:0);if(count){byte(9);byte(8);byte(7);byte(6);byte(5);}
}
int main(){
 for(int scenario=0;scenario<6;++scenario){
  double managerRaw[(sizeof(Access)+7)/8];memset(managerRaw,0,sizeof(managerRaw));Access& m=*reinterpret_cast<Access*>(managerRaw);
  double streamRaw[(sizeof(CDataInputStream)+7)/8];memset(streamRaw,0,sizeof(streamRaw));CDataInputStream& s=*reinterpret_cast<CDataInputStream*>(streamRaw);
  double classRaw[(sizeof(ClassEntry)+7)/8];memset(classRaw,0,sizeof(classRaw));ClassEntry* cls=reinterpret_cast<ClassEntry*>(classRaw);cls->first=100;
  m.DefClassInfo._M_start=cls;m.DefClassInfo._M_finish=cls+1;m.DefClassInfo._M_end_of_storage._M_data=cls+1;
  m.DependencyCheckCRC=scenario==4?0:42;m.UseSafeBinaryFormat=false;GDefStringTable.RandomID=123;
  packet(scenario==1 || scenario==4?99:42,scenario==2?777:123,scenario==3?0:3);
  s.CurrentSourcePtr=input;s.SourceChunkBytesRemaining=len;s.StreamSize=len;s.StreamPos=scenario==0?len:0;
  calls[0]=0;bool ok=m.LoadBinaryDefinitions(s);
  if(scenario<3){
   if(ok || calls[0] || m.UseSafeBinaryFormat || m.InstantiatedDefs.size())return 1;
   if(s.StreamPos!=(unsigned long)(scenario==0?len:scenario==1?5:9))return 2;
  }else{
   if(!ok || !m.UseSafeBinaryFormat || s.StreamPos!=(unsigned long)len || s.SourceChunkBytesRemaining || s.CurrentSourcePtr!=input+len)return 3;
   if(scenario==3){if(strcmp(calls,"x") || m.InstantiatedDefs.size() || m.CompressedOffsets.size() || m.CompressedDefData.size())return 4;}
   else{
    if(strcmp(calls,"xcpdtthlhcpdtthlcpdtthlhs")!=0){printf("calls=%s\n",calls);return 5;}
    if(m.InstantiatedDefs.size()!=3 || m.NameCRCToIndexMap.size()!=2 || m.NameCRCToIndexMap.Dirty)return 6;
    for(int i=0;i<3;++i){CDefinitionManager::CInstantiatedDefInfo* d=m.InstantiatedDefs[i];if(d->ClassName.TablePos!=(i==1?11:10) || d->InstantiationName.TablePos!=(i==2?21:20) || d->ClassIndex!=7+i || d->PDef.Object)return 7;delete d;}
    if(cls->second.GlobalIndices.size()!=2 || cls->second.GlobalIndices[0]!=0 || cls->second.GlobalIndices[1]!=2)return 8;
    if(m.NameCRCToIndexMap[0].first!=200 || m.NameCRCToIndexMap[0].second!=2 || m.NameCRCToIndexMap[1].first!=300 || m.NameCRCToIndexMap[1].second!=0)return 9;
    if(m.CompressedOffsets.size()!=2 || m.CompressedOffsets[0].first!=0 || m.CompressedOffsets[0].second!=3 || m.CompressedOffsets[1].first!=3 || m.CompressedOffsets[1].second!=2 || m.CompressedDefData.size()!=5 || memcmp(m.CompressedDefData._M_start,"\x09\x08\x07\x06\x05",5))return 10;
   }
  }
  free(m.InstantiatedDefs._M_start);free(m.NameCRCToIndexMap._M_start);free(m.CompressedOffsets._M_start);free(m.CompressedDefData._M_start);free(cls->second.GlobalIndices._M_start);
 }
 puts("LOAD_BINARY_DEFS_PASS");return 0;
}