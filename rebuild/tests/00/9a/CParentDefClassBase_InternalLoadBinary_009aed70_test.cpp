#include "fable_def_class_base.h"
#include "fable_data_input_stream.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int baseCalls;
void CDefClassBase::InternalLoadBinary(CDataInputStream& stream){++baseCalls;stream.CurrentSourcePtr+=3;stream.StreamPos+=3;if(stream.SourceChunkBytesRemaining>=3)stream.SourceChunkBytesRemaining-=3;}
void CDataInputStream::ReadWithSrcChunkOverflow(void* p,long n){memcpy(p,CurrentSourcePtr,n);CurrentSourcePtr+=n;StreamPos+=n;SourceChunkBytesRemaining=0;}
int main(){double defraw[8],streamraw[4];CParentDefClassBase* def=reinterpret_cast<CParentDefClassBase*>(defraw);CDataInputStream* stream=reinterpret_cast<CDataInputStream*>(streamraw);
 for(int n=0;n<=2;++n)for(int overflow=0;overflow<2;++overflow){
 memset(defraw,0,sizeof(defraw));memset(streamraw,0,sizeof(streamraw));unsigned char input[32]={0};unsigned short count=(unsigned short)n;memcpy(input+3,&count,2);
 unsigned long words[]={0xffffffffUL,7,0xffffffffUL,0x80000000UL,9,4};memcpy(input+5,words,24);
 stream->CurrentSourcePtr=input;stream->SourceChunkBytesRemaining=overflow?0:32;baseCalls=0;def->SubDefInfoMap.Dirty=true;
 def->CParentDefClassBase::InternalLoadBinary(*stream);
 if(baseCalls!=1 || stream->StreamPos!=5+12*n || def->SubDefInfoMap.Dirty || def->SubDefInfoMap.size()!=(unsigned)n)return 1;
 for(int i=0;i<n;++i)if(def->SubDefInfoMap[i].first!=words[3*i] || (unsigned long)def->SubDefInfoMap[i].second.DefIndex!=words[3*i+1] || (unsigned long)def->SubDefInfoMap[i].second.OriginalParentDefIndex!=words[3*i+2])return 2;
 free(def->SubDefInfoMap._M_start);
 }
 puts("PARENT_LOAD_PASS");return 0;}