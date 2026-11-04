#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_game_text_bank.h"
// Cases (inputs and the independent STLport order) come from NativeBankBlockOrder_model.py.
static void require(bool ok,int line){if(!ok){fprintf(stderr,"BANK_BLOCK_ORDER_FAILURE line=%d\n",line);exit(80);}}
#define CHECK(x) require((x)!=0,__LINE__)
extern "C" void __fastcall InitOrder(void*,void*);
#pragma comment(linker,"/alternatename:@InitOrder@8=?InitBlockOrder@CBankFile@@AAEXXZ")
static unsigned* Words(void* p){return static_cast<unsigned*>(p);}
static unsigned Next(FILE* f){unsigned v=0;CHECK(fread(&v,4,1,f)==1);return v;}
int main(int argc,char** argv){
 CHECK(argc==2);FILE* f=fopen(argv[1],"rb");CHECK(f!=0);unsigned count=Next(f),cases=0,heap=0,packedCases=0,prefilled=0;
 static unsigned char bank[0x110],before[0x110];
 for(unsigned c=0;c<count;++c){
  unsigned n=Next(f),packed=Next(f),prefill=Next(f),heapCase=Next(f),bias=Next(f),bits=Next(f),nwords=Next(f),i;
  unsigned* words=nwords?(unsigned*)malloc(4*nwords):0;for(i=0;i<nwords;++i)words[i]=Next(f);
  unsigned char* data=(unsigned char*)malloc(12*n+1);CHECK(data!=0);
  for(i=0;i<n;++i){unsigned off=Next(f),size=Next(f),valid=Next(f);memcpy(data+12*i,packed?"\xef\xbe\xad\xde":(const char*)&off,4);memcpy(data+12*i+4,&size,4);data[12*i+8]=0x33;data[12*i+9]=(unsigned char)valid;data[12*i+10]=data[12*i+11]=0xee;}
  unsigned nexp=Next(f);unsigned* want=(unsigned*)malloc(4*nexp+4);for(i=0;i<nexp;++i)want[i]=Next(f);
  memset(bank,0x6b,sizeof(bank));Words(bank+4)[0]=n;Words(bank+0x20)[0]=(unsigned)data;Words(bank+0x20)[1]=Words(bank+0x20)[2]=(unsigned)(data+12*n);
  unsigned* pk=Words(bank+0x38);pk[0]=(unsigned)words;pk[1]=packed?n-1:0;bank[0x40]=(unsigned char)(packed?bits:0);pk[3]=packed?bias:0;
  unsigned* order=Words(bank+0xe0);unsigned* old=0;
  if(prefill){old=(unsigned*)malloc(12);old[0]=7;old[1]=3;order[0]=(unsigned)old;order[1]=(unsigned)(old+2);order[2]=(unsigned)(old+3);}else order[0]=order[1]=order[2]=0;
  unsigned char* dataCopy=(unsigned char*)malloc(12*n+1);memcpy(dataCopy,data,12*n);unsigned* wordCopy=(unsigned*)malloc(4*nwords+4);if(nwords)memcpy(wordCopy,words,4*nwords);memcpy(before,bank,sizeof(bank));
  InitOrder(bank,0);
  for(i=0;i<sizeof(bank);++i)if(i<0xe0||i>=0xec)CHECK(bank[i]==before[i]);
  CHECK(!memcmp(data,dataCopy,12*n)&&(!nwords||!memcmp(words,wordCopy,4*nwords)));
  unsigned* begin=(unsigned*)order[0];unsigned* end=(unsigned*)order[1];unsigned* cap=(unsigned*)order[2];
  if(prefill){CHECK(begin==old&&end==old+2&&cap==old+3&&old[0]==7&&old[1]==3);++prefilled;}
  else{CHECK((unsigned)(end-begin)==nexp&&cap>=end&&(nexp||!begin));for(i=0;i<nexp;++i)CHECK(begin[i]==want[i]);
   // Independent of tie order: ascending by offset.
   for(i=1;i<nexp;++i){unsigned a=packed?(*(CPackedUIntArray*)(bank+0x38))[begin[i-1]-1]:Words(data+12*begin[i-1])[0],b=packed?(*(CPackedUIntArray*)(bank+0x38))[begin[i]-1]:Words(data+12*begin[i])[0];CHECK(a<=b);}}
  free(begin);free(words);free(data);free(want);free(dataCopy);free(wordCopy);heap+=heapCase;packedCases+=packed;++cases;
 }
 fclose(f);CHECK(heap==6&&prefilled==2);printf("BANK_BLOCK_ORDER_PASS cases=%u packed=%u heap=%u prefilled=%u\n",cases,packedCases,heap,prefilled);return 0;
}
