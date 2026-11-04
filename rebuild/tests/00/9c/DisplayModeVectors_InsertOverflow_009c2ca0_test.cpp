#include <new>
#include "fable_surface.h"
#include "fable_stlport_uninitialized.h"
#include <stdio.h>
static int failures,scenarios;
static void check(bool b){if(!b)++failures;}
template<class T> T make(long bits){return static_cast<T>(bits);}
template<> CPixelFormat make<CPixelFormat>(long bits){CPixelFormat result;result.FormatIndex=bits;return result;}
template<class T> long bits(const T& v){return static_cast<long>(v);}
template<> long bits<CPixelFormat>(const CPixelFormat& v){return v.FormatIndex;}
template<class T> void run(){
 const int counts[]={0,1,2,7,16};const int fills[]={0,1,2,5,17};
 for(int c=0;c<5;++c)for(int pos=0;pos<=counts[c];++pos)for(int f=0;f<5;++f)for(int alias=0;alias<3;++alias)for(int extra=0;extra<2;++extra)for(int end=0;end<2;++end){
  int oldCount=counts[c],fill=fills[f];if(end && pos!=oldCount)continue;
  _STL::vector<T> v;int capacity=oldCount+extra*3;
  v._M_start=capacity?static_cast<T*>(malloc(capacity*sizeof(T))):0;
  v._M_finish=v._M_start?v._M_start+oldCount:0;v._M_end_of_storage._M_data=v._M_start?v._M_start+capacity:0;
  long expected[64];
  for(int i=0;i<oldCount;++i){T value=make<T>(i*31-77);new(v._M_start+i) T(value);}
  T external=make<T>(-12345);const T* value=&external;if(oldCount && alias)value=&v._M_start[alias==1?0:oldCount-1];long inserted=bits(*value);
  int n=0;for(int a=0;a<pos;++a)expected[n++]=a*31-77;for(int b=0;b<fill;++b)expected[n++]=inserted;for(int d=pos;d<oldCount;++d)expected[n++]=d*31-77;
  v._M_insert_overflow(v._M_start?v._M_start+pos:0,*value,_STL::__false_type(),fill,end!=0);
  check(v.size()==static_cast<unsigned>(oldCount+fill));check(v.capacity()==static_cast<unsigned>(oldCount+(oldCount>fill?oldCount:fill)));
  for(int j=0;j<n;++j)check(bits(v[j])==expected[j]);++scenarios;
 }
}
int main(){run<CPixelFormat>();run<ESurfaceMultisampleType>();printf("DISPLAY_VECTOR_GROWTH_BEHAVIOR_%s scenarios=%d failures=%d\n",failures?"FAIL":"PASS",scenarios,failures);return failures?1:0;}
