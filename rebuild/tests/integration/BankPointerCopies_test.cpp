#include "fable_counted_pointer.h"
#include <stdio.h>
#include <string.h>
#include <new>
class CGraphicDataBank;class CMeshDataBank;
template<class T> static int RunCopy(){
 const unsigned long counts[]={0,1,2,0xffffffffUL};int cases=0;
 for(int data=0;data<3;++data)for(int info=0;info<3;++info)
 for(int a=0;a<4;++a)for(int b=0;b<4;++b){
  union{double align;char bytes[sizeof(CCountedPointer<T>)];} srcStore,dstStore;
  union{double align;char bytes[sizeof(CCPPointerInfo)];} infoStore[2];
  char dataStore[2][16];memset(&srcStore,0xa5,sizeof(srcStore));memset(&dstStore,0xa5,sizeof(dstStore));memset(&infoStore,0xa5,sizeof(infoStore));
  T* dataChoices[]={0,reinterpret_cast<T*>(dataStore[0]),reinterpret_cast<T*>(dataStore[1])};
  CCPPointerInfo* infoChoices[]={0,reinterpret_cast<CCPPointerInfo*>(&infoStore[0]),reinterpret_cast<CCPPointerInfo*>(&infoStore[1])};
  CCountedPointer<T>* source=reinterpret_cast<CCountedPointer<T>*>(&srcStore);
  CCountedPointer<T>* dest=reinterpret_cast<CCountedPointer<T>*>(&dstStore);
  source->Data=dataChoices[data];source->Info=infoChoices[info];
  infoChoices[1]->RefCount=counts[a];infoChoices[2]->RefCount=counts[b];
  const unsigned long expected1=counts[a]+(info==1);const unsigned long expected2=counts[b]+(info==2);
  CCountedPointer<T>* result=new (dest) CCountedPointer<T>(*source);
  if(result!=dest || dest->Data!=dataChoices[data] || dest->Info!=infoChoices[info]){printf("COPY_FAIL data=%d info=%d a=%d b=%d result=%p dest=%p gotdata=%p expecteddata=%p gotinfo=%p expectedinfo=%p\n",data,info,a,b,result,dest,dest->Data,dataChoices[data],dest->Info,infoChoices[info]);return 1;}
  if(infoChoices[1]->RefCount!=expected1 || infoChoices[2]->RefCount!=expected2)return 2;
  for(unsigned int j=4;j<sizeof(CCPPointerInfo);++j){if((unsigned char)infoStore[0].bytes[j]!=0xa5){printf("NEIGHBOR_FAIL1 %d %d %d %d j=%u val=%x\n",data,info,a,b,j,(unsigned char)infoStore[0].bytes[j]);return 3;}if((unsigned char)infoStore[1].bytes[j]!=0xa5){printf("NEIGHBOR_FAIL2 %d %d %d %d j=%u val=%x\n",data,info,a,b,j,(unsigned char)infoStore[1].bytes[j]);return 4;}}
  source->Info=0;dest->Info=0;++cases;
 }
 return cases;
}
int main(){int a=RunCopy<CGraphicDataBank>(),b=RunCopy<CMeshDataBank>();if(a!=144 || b!=144){printf("COPY_COUNT_FAIL %d %d\n",a,b);return 1;}printf("BANK_POINTER_COPIES_BEHAVIOR_PASS cases=%d\n",a+b);return 0;}
