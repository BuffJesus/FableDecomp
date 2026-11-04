#include "fable_basic_string.h"
#include <string.h>
template<class T> void CBasicString<T>::Append(T character,unsigned long length){
 ExtendData(StringLength+length);
 T* end=PData+StringLength;
 for(unsigned long i=0;i<length;++i)end[i]=character;
 unsigned long newLength=StringLength+length;
 StringLength=newLength;
 PData[newLength]=0;
 unsigned long clearFrom=StringLength+1;
 memset(PData+clearFrom,0,((clearFrom+3)&~3)-clearFrom);
}
template void CBasicString<char>::Append(char character,unsigned long length);
