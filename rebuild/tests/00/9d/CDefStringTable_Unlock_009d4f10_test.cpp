#include "fable_def_string.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned reads,insertions,destructions;
static unsigned char payload[128];static unsigned payloadLength;
static char keys[8][32];static long positions[8];
static CDefStringTable* table;
CCharString::CCharString(const char* text,long length){
    if(length<0)length=strlen(text);
    storage_=(CCharStringData*)calloc(1,sizeof(CCharStringData));
    storage_->Data.StringLength=length;
    storage_->Data.PData=(char*)malloc(length+1);
    memcpy(storage_->Data.PData,text,length);storage_->Data.PData[length]=0;
}
CCharString::~CCharString(){++destructions;free(storage_->Data.PData);free(storage_);}
const void* CMemoryDataOutputStream::PeekData()const{++reads;return payload;}
unsigned long CMemoryDataOutputStream::GetLength()const{++reads;return payloadLength;}
namespace _STL {
template<>long& map<CCharString,long>::operator[](const CCharString& key){
    if(this!=&table->StringMap||!table->Locked||table->ReadOnly)abort();
    const CCharStringData* data=*reinterpret_cast<CCharStringData* const*>(&key);
    for(unsigned i=0;i<insertions;++i)if(!strcmp(keys[i],data->Data.PData))return positions[i];
    strcpy(keys[insertions],data->Data.PData);return positions[insertions++];
}
}
static void add(const char* text){unsigned crc=0x12345678;memcpy(payload+payloadLength,&crc,4);payloadLength+=4;unsigned n=strlen(text)+1;memcpy(payload+payloadLength,text,n);payloadLength+=n;}
static void run(bool locked,bool readonly,unsigned count){
    unsigned char bytes[sizeof(CDefStringTable)];memset(bytes,0,sizeof(bytes));table=reinterpret_cast<CDefStringTable*>(bytes);
    table->Locked=locked;table->ReadOnly=readonly;table->StringCount=77;table->RandomID=0xabcdef;
    reads=insertions=destructions=0;payloadLength=4;memset(payload,0,sizeof(payload));
    if(count){add("alpha");if(count>1)add("");if(count>2)add("alpha");}
    table->Unlock();
    bool active=locked&&!readonly;
    if(table->Locked!=(active?false:locked)||table->ReadOnly!=readonly||table->StringCount!=77||table->RandomID!=0xabcdef)abort();
    if(!active){if(reads||insertions||destructions)abort();}
    else{if(reads!=2||destructions!=count||insertions!=(count>1?2:count))abort();if(count&&strcmp(keys[0],"alpha"))abort();if(count&&positions[0]!=(count>2?19:4))abort();if(count>1&&(keys[1][0]||positions[1]!=14))abort();}
}
int main(){run(false,false,3);run(false,true,3);run(true,true,3);run(true,false,0);run(true,false,1);run(true,false,2);run(true,false,3);puts("DEFSTRING_UNLOCK_OK cases=7");return 0;}
