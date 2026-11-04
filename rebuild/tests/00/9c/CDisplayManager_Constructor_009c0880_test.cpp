#include <new>
#include "fable_display_viewport.h"
#include "fable_stlport_uninitialized.h"
#include <stdio.h>
#include <string.h>
static CDisplayManager* display;static int growth,mutation,pixels,failures;
static const int formats[]={25,31,21,23,24,22};
static void check(bool b){if(!b)++failures;}
CBaseClass::CBaseClass(){} CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){} CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CWideString::CWideString(){storage_=0;} CWideString::~CWideString(){}
void CSurface::Uninitialise(){} void CTexture::Uninitialise(){}
namespace _STL {
 template<> list<CDeviceResetCallback*>::~list(){}
 template<> list<CBusyResourceManager::CPendingQuery>::~list(){}
 template<> list<CVsyncCallbackTimer::CPendingQuery>::~list(){}
}
CBusyResourceManager::CBusyResourceManager():PendingFrame(1),FinishedFrame(0){}
CBusyResourceManager::~CBusyResourceManager(){}
CVsyncCallbackTimer::~CVsyncCallbackTimer(){}
CDisplayManager::~CDisplayManager(){}
void CPixelFormat::Initialise(D3DFORMAT format){
 check(pixels<6 && format==formats[pixels++]);FormatIndex=static_cast<long>(format)^0x55;
 if(mutation){unsigned long bits=0x3f000000+mutation;memcpy(&display->Gamma,&bits,4);}
}
template<class T> static void grow(_STL::vector<T>* v,T* where,const T& value,size_t count,bool atEnd){
 check(where==v->_M_finish && v->_M_finish==v->_M_end_of_storage._M_data && count==1 && atEnd);
 unsigned n=v->size();unsigned capacity=growth==0?n+1:growth==1?(n?n*2:1):n+5;
 T* data=static_cast<T*>(malloc(capacity*sizeof(T)));if(n)memcpy(data,v->_M_start,n*sizeof(T));memcpy(data+n,&value,sizeof(T));free(v->_M_start);v->_M_start=data;v->_M_finish=data+n+1;v->_M_end_of_storage._M_data=data+capacity;
}
namespace _STL {
 template<> void vector<CPixelFormat>::_M_insert_overflow(CPixelFormat* p,const CPixelFormat& v,const __false_type&,size_t n,bool end){grow(this,p,v,n,end);}
 template<> void vector<ESurfaceMultisampleType>::_M_insert_overflow(ESurfaceMultisampleType* p,const ESurfaceMultisampleType& v,const __false_type&,size_t n,bool end){grow(this,p,v,n,end);}
}
static void set(unsigned char* expected,unsigned offset,unsigned long value){memcpy(expected+offset,&value,4);}
static void pointer(unsigned char* expected,unsigned offset,void* value){set(expected,offset,reinterpret_cast<unsigned long>(value));}
static void list(unsigned char* expected,unsigned offset,unsigned long* storage){
 _STL::_List_node_base* head=reinterpret_cast<_STL::_List_node_base*>(storage[offset/4]);check(head && head->_M_next==head && head->_M_prev==head);pointer(expected,offset,head);
}
int main(){unsigned scenarios=0;unsigned long system[0x180/4];
 for(unsigned fill=0;fill<256;++fill)for(growth=0;growth<3;++growth)for(mutation=0;mutation<4;++mutation){
  unsigned long storage[0x2c8/4];unsigned char expected[sizeof(storage)];memset(storage,fill,sizeof(storage));memcpy(expected,storage,sizeof(storage));display=reinterpret_cast<CDisplayManager*>(storage);pixels=0;
  CDisplayManager* result=new(storage) CDisplayManager(*reinterpret_cast<CSystemManager*>(system));check(result==display && pixels==6);
  check(storage[0]!=0);set(expected,0,storage[0]);pointer(expected,4,system);set(expected,8,0);
  for(unsigned at=0xc;at<=0x3c;at+=0x10){check(storage[at/4]!=0 && storage[at/4]==storage[3]);set(expected,at,storage[at/4]);set(expected,at+4,0);set(expected,at+8,0);}
  for(unsigned z=0x4c;z<=0x58;z+=4)set(expected,z,0);
  set(expected,0x1a8,0xffffffff);set(expected,0x1ac,0xffffffff);set(expected,0x258,0);set(expected,0x25c,0);set(expected,0x294,0);list(expected,0x264,storage);
  check(display->BackBufferFormats.size()==6 && display->MultisampleTypes.size()==16);
  for(unsigned f=0;f<6;++f)check(display->BackBufferFormats[f].FormatIndex==(formats[f]^0x55));
  for(unsigned m=0;m<16;++m)check(display->MultisampleTypes[m]==(m?m+1:0));
  memcpy(expected+0x298,storage+0x298/4,24);list(expected,0x2b0,storage);list(expected,0x2b4,storage);set(expected,0x2b8,1);set(expected,0x2bc,0);set(expected,0x2c4,mutation?0x3f000000+mutation:0x3f800000);
  check(memcmp(expected,storage,sizeof(storage))==0);
  free(reinterpret_cast<void*>(storage[0x264/4]));free(reinterpret_cast<void*>(storage[0x2b0/4]));free(reinterpret_cast<void*>(storage[0x2b4/4]));free(display->BackBufferFormats._M_start);free(display->MultisampleTypes._M_start);++scenarios;
 }
 printf("DISPLAY_CONSTRUCTOR_BEHAVIOR_%s scenarios=%u failures=%d\n",failures?"FAIL":"PASS",scenarios,failures);return failures?1:0;
}
