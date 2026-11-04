// Recovered from retail instructions; shared ABI. Validated by native owned-open gate.
#include <new>
#include <string.h>
#include "fable_basic_string.h"
template<class T> void CBasicString<T>::SetString(unsigned long length,T character)
{
 if(PData) {delete[] PData;PData=0;}
 unsigned long capacity=(length+4)&~3UL;
 T* data=new T[capacity];
 DataLength=capacity;
 PData=data;
 if(length>0)memset(PData,character,length);
 PData[length]=0;
 StringLength=length;
 PData[length]=0;
 unsigned long start=StringLength+1;
 memset(PData+start,0,((start+3)&~3UL)-start);
}
template void CBasicString<char>::SetString(unsigned long,char);
